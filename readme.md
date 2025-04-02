# 🧠 AI Story Generator with C++ and llama.cpp

This is a simple console-based application written in C++ that integrates with [llama.cpp](https://github.com/ggerganov/llama.cpp) to generate AI-powered stories based on user prompts. Stories are stored in a custom singly linked list structure, allowing for basic operations like insertion, deletion, and swapping.

## 📚 Features

- Generate a story using an LLM via llama.cpp
- Store multiple stories in a custom `List` class
- View saved stories
- Swap positions of saved stories
- Delete specific stories
- Simple loading spinner and generation feedback

## 🛠 Requirements

- A compiled version of [`llama.cpp`](https://github.com/ggerganov/llama.cpp)
- A GGUF model (e.g., [`deepseek-llm-7b-chat.Q4_K_M.gguf`](https://huggingface.co/deepseek-ai/deepseek-llm-7b-chat))
- C++17 or later
- Windows OS (due to use of `taskkill`, `filesystem`, and `.exe` paths)

## ⚙️ How It Works

1. The user inputs a theme for the story.
2. The prompt is passed to llama.cpp via command line.
3. The story is saved to `output.txt`.
4. The application reads the result and stores it in a linked list.
5. The user can interact with the saved stories via a menu.


> ⚠️ Make sure to update the paths to `llama-cli.exe` and the model file in the `generateStory()` function.

## 📌 Notes

- You can increase the waiting time or change the spinner interval inside `waitForFileCompletion()`.
- This project uses `std::thread` and `std::filesystem`, so compile with C++17 or higher.
- This is a proof-of-concept and can be extended with features like:
  - Saving to file
  - GUI integration
  - JSON export
  - More robust error handling