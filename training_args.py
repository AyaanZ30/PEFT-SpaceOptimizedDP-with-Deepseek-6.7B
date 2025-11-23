from transformers import TrainingArguments
from . import config

def get_training_arguments():
    """
    Returns the configured TrainingArguments object.
    """
    return TrainingArguments(
        output_dir=config.OUTPUT_DIR,
        overwrite_output_dir=True,

        # Checkpointing and Evaluation
        save_strategy="epoch",
        save_total_limit=3,
        load_best_model_at_end=True,
        metric_for_best_model="eval_loss",
        greater_is_better=False,

        # Training Hyperparameters
        num_train_epochs=config.TRAINING_EPOCHS,
        per_device_train_batch_size=1,
        gradient_accumulation_steps=8,
        per_device_eval_batch_size=1,

        # Optim Settings
        learning_rate=config.LEARNING_RATE,
        weight_decay=0.01,
        max_grad_norm=1.0,
        warmup_ratio=0.1,

        optim="paged_adamw_8bit",
        lr_scheduler_type="cosine_with_restarts",
        fp16=True,                                  # Recommended for GPU
        gradient_checkpointing=True,

        logging_steps=20,
        eval_strategy="epoch",
        
        seed=42,
        report_to="none",
        ddp_find_unused_parameters=False,
    )