import json
import re
from datasets import Dataset

def load_file(path: str) -> list:
    """
    Loads raw data from a JSONL file.
    """
    print(f"Loading raw data from {path}...")
    
    raw_data = []
    
    with open(path, 'r', encoding='utf-8') as f:
        for line in f:
            raw_data.append(json.loads(line))
            
    print(f"Loaded {len(raw_data)} raw records.")
    return raw_data


def clear_extra_text(code_block: str) -> str:
    """
    Removes comments and excessive newlines from code blocks.
    """
    code_block = re.sub(r'//.*', '', code_block)
    code_block = re.sub(r'/\*.*?\*/', '', code_block, flags=re.DOTALL)
    code_block = re.sub(r'\n\s*\n', '\n', code_block)
    return code_block.strip()


def format_data_for_training(raw_data_list: Dataset) -> Dataset:
    """
    Formats the raw data into the Deepseek Chat Template for SFT training.
    """
    formatted_data = []
    
    for item in raw_data_list:
        try:
            problem = item.get('problem_name', '').strip()
            prompt = item.get('prompt', '').strip()
            solution = item.get('space_optimal_solution', '').strip()

            solution = clear_extra_text(solution)
            
            # Deepseek-Coder instruction template
            text = f"""<|system|>
            You are an expert C++ programmer specializing in dynamic programming. Provide only the most space-optimized C++ function. No main() function or examples.
            Problem: {problem}
            <|user|>
            {prompt}
            <|end|>
            <|assistant|>
            ```cpp
            {solution}
            ```"""
            formatted_data.append({"text": text})
        except Exception as e:
            print(f"An error occurred processing item: {e}")
            
    return Dataset.from_list(formatted_data)