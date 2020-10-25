# spleeter-tflite-convert
Things to know:
 * those are python 3 scripts that require tensorflow's latest version (I installed it with pip3)
 * the first one, "export_model.py", will create a "saved model" from the checkpoint
 * the second one, "tofreezelite.py", will convert the "saved model" to "converted_model.tflite" (which is roughly 150MB with the 4stems checkpoint)
 * the path to the model checkpoint is hardcoded in the first script:

I have my model checkpoint at this location : /Users/tinou/Desktop/4stems
	
that prefix variable must be the full path of the .meta file, up to the extension.

 * the input and output tensor names are hardcoded for the 4stems model checkpoint, if you plan on converting another one, you will have to manually change the output tensor names (I found those by looking into spleeterpp python script that didn't work for me).
 * you will need TensorFlowLiteSelectTfOps in addition to TensorFlowLiteC when using it.

It uses a *lot* of RAM, you'll only be able to process small chunks of audio at a time on iOS (or Android). Sounds 'ok-ish' with 2s chuncks, with cross-faded overlap (4096 samples of overlap)
I'm interested if anyone finds a way to modify the checkpoint to make it use static-sized tensors. As is it is, acceleration (GPU/Metal or coreml) isn't possible.

# Note

This directory contains files and work from https://github.com/tinoucas/spleeter-tflite-convert