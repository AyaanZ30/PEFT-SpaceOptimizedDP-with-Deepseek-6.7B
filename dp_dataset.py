import json
import os
import re

def extract_metadata(text_block: str):
    """Extracts complexities, explanation, and the clean code from a comment block."""
    explanation_match = re.search(r"@explanation:\s*(.*)", text_block, re.IGNORECASE)
    complexity_match = re.search(r"@complexity:\s*(.*)", text_block, re.IGNORECASE)

    explanation = explanation_match.group(1).strip() if explanation_match else "No explanation provided"
    complexity = complexity_match.group(1).strip() if complexity_match else "No complexity provided"

    # Remove the metadata comments to get the clean code
    code = re.sub(r"//\s*@explanation:.*", "", text_block, re.IGNORECASE)
    code = re.sub(r"//\s*@complexity:.*\n?", "", code, flags=re.IGNORECASE).strip()

    return code, explanation, complexity

def extract_prompt(full_code: str):
    """Extracts the problem prompt from the // PROMPT: comment in the code."""
    prompt_match = re.search(r"//\s*PROMPT:\s*(.*)", full_code, re.IGNORECASE)
    return prompt_match.group(1).strip() if prompt_match else "No prompt provided."

def extract_first_function(code_block: str):
    """
    Extracts the first complete C++ function from a block of code.
    It works by finding the first opening brace '{' and then counting
    nested braces to find its corresponding closing brace '}'.
    """
    try:
        first_brace_index = code_block.index('{')
    except ValueError:
        # No function found, return the block as is
        return code_block

    brace_count = 1
    # Start searching from the character after the first brace
    for i, char in enumerate(code_block[first_brace_index + 1:]):
        if char == '{':
            brace_count += 1
        elif char == '}':
            brace_count -= 1
        
        if brace_count == 0:
            # We found the matching closing brace
            # The slice includes the function body up to the closing brace
            return code_block[:first_brace_index + 1 + i + 1]
    
    # If no matching brace is found (e.g., malformed code), return the original block
    return code_block

def create_dp_datasets(code_folder: str, full_dataset_file: str, finetuning_file: str):
    """
    Parses C++ files to generate two JSON datasets:
    1. A comprehensive dataset with naive/optimized solutions and metadata.
    2. A streamlined dataset specifically for fine-tuning LLMs (prompt -> solution).
    """
    full_dataset = []
    finetuning_dataset = []

    if not os.path.exists(code_folder):
        raise FileNotFoundError(f"The provided folder path '{code_folder}' does not exist.")

    for file in os.listdir(code_folder):
        if not file.endswith(".cpp"):
            continue

        problem_name = file.replace(".cpp", "").replace("_", " ").title()
        filepath = os.path.join(code_folder, file)

        try:
            with open(filepath, "r", encoding="utf-8") as f:
                code = f.read()

            naive_marker = r"//\s*NAIVE"
            optimized_marker = r"//\s*OPTIMIZED"
            end_marker = r"(//\s*PROMPT:|int\s+main\s*\()"

            m_naive = re.search(naive_marker, code, re.IGNORECASE)
            m_opt = re.search(optimized_marker, code, re.IGNORECASE)
            m_end = re.search(end_marker, code, re.IGNORECASE)
            prompt = extract_prompt(code)

            if m_naive and m_opt and m_naive.start() < m_opt.start():
                naive_block_raw = code[m_naive.end():m_opt.start()].strip()
                
                optimized_block_end = m_end.start() if m_end else len(code)
                optimized_block_raw = code[m_opt.end():optimized_block_end].strip()

                # --- Process Naive Block ---
                naive_code, naive_explanation, naive_complexity = extract_metadata(naive_block_raw)

                # --- Process Optimized Block ---
                # Get metadata and all the code in the optimized block
                temp_optimized_code, optimized_explanation, optimized_complexity = extract_metadata(optimized_block_raw)
                # Isolate only the first function from the code
                final_optimized_code = extract_first_function(temp_optimized_code)
                
                main_start_match = re.search(r"int\s+main\s*\(", code, re.IGNORECASE)
                main_function_code = "None"
                if main_start_match:
                    main_block_raw = code[main_start_match.start():]
                    main_function_code = extract_first_function(main_block_raw).strip()


                # 1. Append to the full dataset
                full_dataset.append({
                    "problem_name": problem_name,
                    "naive_code": naive_code,
                    "optimized_code": final_optimized_code, # Use the isolated function
                    "naive_explanation": naive_explanation,
                    "optimized_explanation": optimized_explanation,
                    "naive_complexity": naive_complexity,
                    "optimized_complexity": optimized_complexity,
                })

                # 2. Append to the fine-tuning dataset
                finetuning_dataset.append({
                    "problem_name": problem_name,
                    "prompt": prompt,
                    "space_optimal_solution": final_optimized_code, # Use the isolated function
                    "main_function" : main_function_code
                })
            else:
                print(f"⚠️ Skipped {file}: Couldn't find NAIVE/OPTIMIZED sections in the correct order.")
        except Exception as e:
            print(f"❌ Error processing file {filepath}: {e}")

    # Write the full dataset to a standard JSON file
    with open(full_dataset_file, "w", encoding="utf-8") as out_f:
        json.dump(full_dataset, out_f, indent=4)
    print(f"✅ Full dataset with {len(full_dataset)} entries created at '{full_dataset_file}'")

    # Write the fine-tuning data to a JSON Lines (.jsonl) file
    with open(finetuning_file, "w", encoding="utf-8") as out_f:
        for entry in finetuning_dataset:
            out_f.write(json.dumps(entry) + "\n")
    print(f"✅ Fine-tuning dataset with {len(finetuning_dataset)} entries created at '{finetuning_file}'")

# --- Main Execution ---
if __name__ == "__main__":
    create_dp_datasets(
        code_folder="dp_problems",
        full_dataset_file="dp_dataset.json",
        finetuning_file="finetuning_data.jsonl"
    )