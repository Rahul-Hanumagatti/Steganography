# Steganography
Steganography using C programming

This project implements an effective steganography technique using the C programming language. It hides secret messages in BMP image files by altering the least significant bits of pixel data, ensuring minimal visual distortion.

Features:

Encoding: Embed messages by modifying pixel data.
Decoding: Extract hidden messages accurately.
Error Handling: Ensures compatibility and size constraints.
Technologies Used:

C Language: Efficient and fast execution.
Bitwise Operations: Precise data manipulation.
File Handling: Binary data read/write operations.
Usage:

Compile the project
gcc steganography.c -o steganography

Encode a message
./steganography encode input.bmp output.bmp "Secret Message"

Decode the message
./steganography decode output.bmp
