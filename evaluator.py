import os
import time
import torch
import psutil
import subprocess
from typing import Dict, Any

def generate_solution(model, tokenizer, problem: str, prompt: str, max_new_tokens: int = 512) -> str:
    """
    Generates a space-optimized C++ solution using the fine-tuned model
    ."""
    
    # Force a starting function signature for better output consistency
    formatted_prompt = f"""<|system|>
    You are an expert C++ programmer specializing in dynamic programming. Provide only the most space-optimized C++ function. No main() function or examples.

    Problem: {problem}
    <|user|>
    {prompt}
    <|end|>
    <|assistant|>
    ```cpp
    int hyper_optimized_approach"""
    
    inputs = tokenizer(formatted_prompt, return_tensors="pt", truncation=True, max_length=1024)
    inputs = {k: v.to(model.device) for k, v in inputs.items()}
    
    with torch.no_grad():
        outputs = model.generate(
            **inputs,
            max_new_tokens=max_new_tokens,
            temperature=0.1,
            do_sample=True,
            pad_token_id=tokenizer.eos_token_id,
            eos_token_id=tokenizer.eos_token_id,
        )
    
    input_length = inputs['input_ids'].shape[1]
    generated_tokens = outputs[0][input_length:]
    generated_text = tokenizer.decode(generated_tokens, skip_special_tokens=True)
    
    full_code = "int hyper_optimized_approach" + generated_text.split('```')[0].strip()
    
    return full_code

def measure_peak_memory(executable_path: str) -> float:
    """
    Measures the peak Resident Set Size (RSS) memory usage of a compiled C++ program in MB.
    """
    if not os.path.exists(executable_path):
        return -1.0
    try:
        process = subprocess.Popen([executable_path], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        p = psutil.Process(process.pid)
        peak_mem = 0
        
        # Monitor memory until the process terminates
        while process.poll() is None:
            peak_mem = max(peak_mem, p.memory_info().rss)
            time.sleep(0.002) # Short delay for accurate sampling
            
        process.communicate() 
        
        # Convert bytes to megabytes
        return peak_mem / (1024 * 1024)
    except Exception as e:
        print(f"Error measuring memory for {executable_path}: {e}")
        return -1.0


def create_cpp_program(solution: str, main_func: str, filename: str):
    """
    Creates a complete C++ program file.
    """
    program = f"""
    #include <iostream>
    #include <vector>
    #include <algorithm>
    #include <climits>
    #include <bits/stdc++.h> // Common competitive programming header
    using namespace std;
    
    {solution}
        
    {main_func}
    """
    
    with open(filename, 'w') as f:
        f.write(program)


def compile_cpp(file: str, executable: str) -> bool:
    """
    Compiles a C++ file using g++.
    """
    result = subprocess.run(
        ['g++', '-std=c++17', '-O2', file, '-o', executable],
        capture_output=True, text=True
    )
    if result.returncode == 0:
        print(f"Compiled: {executable}")
        return True
    else:
        print(f"Failed to compile {file}: {result.stderr}")
        return False


def test_single_case(model, tokenizer, sample: Dict[str, Any]) -> Dict[str, Any]:
    """
    Tests one problem sample by generating a solution, compiling it, and measuring memory against ground truth.
    """
    
    problem, prompt = sample['problem_name'], sample['prompt']
    gt_solution, main_func = sample['space_optimal_solution'], sample['main_function']
    
    print(f"\nTesting: {problem}")
    
    generated = generate_solution(model, tokenizer, problem, prompt)
    
    print(f"Generated: {generated[:100]}...")
    
    create_cpp_program(generated, main_func, "gen.cpp")
    create_cpp_program(gt_solution, main_func, "gt.cpp")

    result = {'problem': problem}
    
    if compile_cpp("gen.cpp", "gen_app") and compile_cpp("gt.cpp", "gt_app"):
        mem_gen = measure_peak_memory("./gen_app")
        mem_gt = measure_peak_memory("./gt_app")
        
        print(f"Memory - Generated: {mem_gen:.2f}MB, Ground Truth: {mem_gt:.2f}MB")
        result.update({'gen_mem': mem_gen, 'gt_mem': mem_gt, 'diff': mem_gen - mem_gt})
        
        # Cleanup executable files
        for f in ["gen.cpp", "gt.cpp", "gen_app", "gt_app"]:
            if os.path.exists(f): os.remove(f)
            
    else:
        result.update({'error': 'Compilation failed'})
    
    return result