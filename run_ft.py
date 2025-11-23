import os
import torch
from datasets import Dataset
from transformers import DataCollatorForLanguageModeling
from trl import SFTTrainer
from huggingface_hub import login

# moudular imports
from . import config
from .data_handler import load_file, format_data_for_training
from .model_config import setup_model_and_tokenizer
from .training_args import get_training_arguments
from .evaluator import test_single_case

# Set environment variables
os.environ['TOKENIZERS_PARALLELISM'] = 'false'
os.environ['PYTORCH_CUDA_ALLOC_CONF'] = 'expandable_segments:true'

def main():
    """
    Orchestrates the data loading, model fine-tuning, and evaluation.
    """
    
    print("Logging into Hugging Face...")
    login(token = config.MY_TOKEN)

    # 2. Data Preparation
    raw_data = load_file(config.JSON_FILE_PATH)
    full_dataset_raw = Dataset.from_list(raw_data)

    train_test_split = full_dataset_raw.train_test_split(
        test_size=0.2, seed=42, shuffle=True
    )
    train_dataset_raw = train_test_split["train"]
    eval_dataset_raw = train_test_split["test"]

    train_dataset = format_data_for_training(train_dataset_raw)
    eval_dataset = format_data_for_training(eval_dataset_raw)

    print(f"Formatted Training samples: {len(train_dataset)}")
    print(f"Formatted Validation samples: {len(eval_dataset)}")

    # 3. Model and Trainer Setup
    model, tokenizer, lora_config = setup_model_and_tokenizer()
    training_args = get_training_arguments()

    trainer = SFTTrainer(
        model=model,
        train_dataset=train_dataset,
        eval_dataset=eval_dataset,
        args=training_args,
        peft_config=lora_config,
        data_collator=DataCollatorForLanguageModeling(
            tokenizer=tokenizer,
            mlm=False,
            pad_to_multiple_of=8
        ),
        max_seq_length=2048 # Adjust as needed
    )
    
    # Training (This is the step that requires a GPU like on Kaggle)
    torch.cuda.empty_cache()
    print("STARTING MODEL TRAINING")
    trainer.train()
    print("TRAINING COMPLETED!")

    final_model_path = f"{config.OUTPUT_DIR}/final_model"
    trainer.model.save_pretrained(final_model_path)
    tokenizer.save_pretrained(final_model_path)
    print(f"Model saved to: {final_model_path}")
    
    print("\n--- Starting Evaluation ---")
    
    # We will only evaluate on a few samples to save time and resources
    evaluation_results = []

    for i in range(min(5, len(train_dataset_raw))):
        sample = train_dataset_raw[i]
        result = test_single_case(model, tokenizer, sample)
        evaluation_results.append(result)
        
    print("\nEvaluation Summary:")
    for res in evaluation_results:
        print(res)


if __name__ == "__main__":
    main()