import torch

MODEL_NAME = "deepseek-ai/deepseek-coder-6.7b-instruct"
MY_TOKEN = ""  # paste your token

JSON_FILE_PATH = '/kaggle/input/data-ft-1/finetuning_data.jsonl'
OUTPUT_DIR = "/kaggle/working/deepseek_spo"

# --- Training and LoRA Hyperparameters ---
R_LORA = 50
LORA_ALPHA = 100
LORA_DROPOUT = 0.03
TRAINING_EPOCHS = 10
LEARNING_RATE = 1e-4

# --- Quantization Config ---
BNB_CONFIG = {
    'load_in_4bit': True,
    'bnb_4bit_use_double_quant': True,
    'bnb_4bit_quant_type': "nf4",
    'bnb_4bit_compute_dtype': torch.float16,
    'bnb_4bit_quant_storage': torch.uint8
}