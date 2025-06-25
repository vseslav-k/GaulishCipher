# 🛡️ GaulishCipher

**GaulishCipher** is a custom file encryption tool written in C++ that uses a position-based, pseudo-random byte transformation algorithm. It is designed for educational or novelty use, not secure production environments.

---

## 🔐 Overview
GaulishCipher was originally inspired by the primitive and popular Caesar cipher. Gaulish cipher seeks to improve upon the Caesar cipher while retaining simular core logic to create a <b>somewhat</b> secure encryption algorithm.

GaulishCipher operates as a **stream cipher variant** that performs **byte-wise encryption** using a combination of fixed parameters ("key") and runtime state. The cipher is **deterministic** and **invertible** with the same key, allowing reliable decryption of previously encrypted data.

---

## 🧪 Core Algorithm Structure

The cipher key is a dot-delimited string with six positive integers:

```
offset.initSalt.posPower.offPower.prevPower.cap
```
The key is inputed by the user.

Each byte is essentially offset by some constant offset value, its own index, and the value of a previous byte (encrypted).


Each parameter of the key controls a part of the internal state transformation and contributes to the complexity of the encryption:

- `offset`: constant value by which each byte will be offset
- `offPower`:  Multiplied with `offset`  before offsetting the current byte by it
- `posPower`: Multiplied with current byte index before offsetting the current byte by it
- `prevPower`: Multiplied with previous (already encrypted) byte value before offsetting the current byte by it
- `initSalt`: Acts as previous byte value for the first byte of the file
- `cap`: mods the sum of all above mentioned offsets before it is applied

So each byte is shifted by a modifier, 
where modifer = (offset * offPower + current byte index * posPower + previous encrypted byte value * prevPower) % cap +1

note that if the current byte index is 0, initSalt is used instead of previous encrypted byte value

---

## 🔄 Magic Numbers

Aside from the previously described "core transformation", each byte also undergoes a "magic number" transformation.
For each byte, a magic number is generated, based on its calculated modifier, index, and their modulo relation to various key values.
This proccess is non-linear and non-continous, however, fully deterministic
For most bytes the magic number will be 0, however for a significant portion, it will be nonzero.

Magic numbers are generated with the `generateMagicNumber` function, which is implement through a series of arbitrary if-return statements

For example:
```
...
if (cap % idx == initSalt % idx) {
	return 4;
}
if ((cap * idx) % 10 == (modifier * idx) % 10) {
	return 10;
}
...
```
see `Cipher::generateMagicNumber(int modifier, int idx)` in GaulishCipher.cpp for all if statements

---

## 🧠 Cryptographic Characteristics

| Property             | Description                                                                        |
|----------------------|------------------------------------------------------------------------------------|
| **Deterministic**    | Same input with same key always gives same output.                                 |
| **Invertible**       | Decryption is possible only with the original key.                                 |
| **Nonlinear**        | Uses conditional logic with modulus and position-dependent branches (magic numbers)|
| **Stateful**         | Output at index `i` depends on previous byte(s), introducing chaining.             |
| **Key-dependent**    | Each part of the key alters control flow paths and transformation logic.           |

---

## ✅ Strengths

- ~2<sup>186</sup> possible keys, making brute force impossible
- Non-linear transformation logic – the use of complex, condition-based branching (via generateMagicNumber) resists algebraic and equation-based cryptanalysis.
- Obscure control flow – hardcoded modular and positional rules obscure the cipher’s internal state evolution, making static reverse engineering nontrivial.
- Dynamic, stateful behavior – the output for each byte depends on previous state and position, defeating simple frequency or statistical analysis.
- High key sensitivity – even minor changes in any of the six key parameters result in entirely different ciphertext, reinforcing avalanche behavior.

---

## ❌ Weaknesses

- Mean byte value of encrypted text can be used to narrow down the value of the key
- **Not cryptographically secure** — no use of secure PRNG, entropy estimation, or proven block cipher structure
- **Not resistant to known plaintext attacks** (e.g. knowing the first few bytes can lead to key leakage)
- Lacks forward secrecy or key derivation

---

## 🚀 Usage

First, download GaulishCipher.exe (source code files are not neccesary to run the exe)

Run the exe file directly by double clicking it. It will prompt you to enter what action you want to take (1 for encrypt, 2 for decrypt), target file path, key, and output file path.

The program supports file paths with forward and backwards slashes. 
The program does not care if the file path is in "double" or 'single' quotes, or no quotes

If you only provide file name with no extension for output path, the program will encrypt as a .gaul file and decrypt as .txt by default.
If you only provide file name, no directory, for output path, the program will default to target path.
The program supports relative paths.

Spaces in a file name can cause undefined behavior. Do not encrypt files with space in their name(might fix this later)

You can also run the program from console. If you do you can use args. Args should be formatted the same as prompted input and need to be written in the following order

`GaulishCipher.exe actionNum targetPath key outPath`

for example

`GaulishCipher.exe 1 C:\Users\vsesl\Documents\msg.txt 2.3.4.5.6.7 encryptedMsg.gaul`

You can also drop the last parameter, in which case the program will default to enc.gaul if you are encrypting and dec.txt if you are decrypting


---

## ⚠️ Legal and Ethical Use

This software is for **educational** and **personal use only**. Do not use GaulishCipher for securing sensitive or production data. It has not been audited or peer-reviewed.

---
