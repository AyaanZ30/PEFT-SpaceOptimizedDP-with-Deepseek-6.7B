import torch
from transformers import AutoModelForCausalLM, AutoTokenizer, BitsAndBytesConfig
from peft import LoraConfig, get_peft_model, prepare_model_for_kbit_training
from . import config

def setup_model_and_tokenizer():
    """
    Sets up the tokenizer, model, and LoRA configuration.
    """
    
    bnb_config = BitsAndBytesConfig(**config.BNB_CONFIG)

    print("Loading tokenizer...")
    tokenizer = AutoTokenizer.from_pretrained(
        config.MODEL_NAME,
        trust_remote_code=True,
        padding_side="right",
        token=config.MY_TOKEN
    )

    if tokenizer.pad_token is None:
        tokenizer.pad_token = tokenizer.eos_token
        tokenizer.pad_token_id = tokenizer.eos_token_id
        
    special_tokens_dict = {}
    tokens_to_add = ["<|system|>", "<|user|>", "<|assistant|>", "<|end|>"]
    if any(t not in tokenizer.get_vocab() for t in tokens_to_add):
        special_tokens_dict["additional_special_tokens"] = tokens_to_add

    if special_tokens_dict:
        tokenizer.add_special_tokens(special_tokens_dict)

    print("Loading Model with 4-bit Quantization")
    model = AutoModelForCausalLM.from_pretrained(
        config.MODEL_NAME,
        quantization_config=bnb_config,
        device_map="auto",
        token=config.MY_TOKEN,
        trust_remote_code=True,
        torch_dtype=torch.float16,
        use_cache=False,
    )
    
    if special_tokens_dict:
        model.resize_token_embeddings(len(tokenizer))

    model = prepare_model_for_kbit_training(model)
    model.config.use_cache = False  # Disable caching for checkpointing

    # LoRA Configuration
    lora_config = LoraConfig(
        r=config.R_LORA,
        lora_alpha=config.LORA_ALPHA,
        target_modules=["q_proj", "k_proj", "v_proj", "o_proj", "gate_proj", "up_proj", "down_proj"],
        lora_dropout=config.LORA_DROPOUT,
        bias="none",
        task_type="CAUSAL_LM",
        modules_to_save=None
    )
    
    model = get_peft_model(model, lora_config)
    model.print_trainable_parameters()
    
    print("Model and LoRA configured successfully!")
    return model, tokenizer, lora_config