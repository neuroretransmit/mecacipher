# mecacipher

A chaos-based, metamorphic block cipher based on second-order cellular automata (MECA).

* 128-bit block size
* Key size of up to 255 bytes
* Metamorphic engine to select second-order rules to evolve based on incoming data
* Configurable rounds for added security
* Key scheduling using a modified version of RC6's scheduling algorithm
    - Globally chaotic (class 4) rules of elementary CA (irreversible) evolve the key

## Output

Configured for 15 rounds

```
plaintext: 00001337 00001337 00001337 00001337
key      : 4d792063 72696d65 20697320 74686174 206f6620 63757269 6f736974 79000000
encrypted: 09caccda d1c7bf5a 30d6a330 89ceacc6
decrypted: 00001337 00001337 00001337 00001337
enc time : 46 microseconds
dec time : 43 microseconds
```

## Future plans

- [ ] Extend block size to 256 bits
- [ ] Metamorphic engine for key scheduling (xor, add, subtract, invert, etc)

## References

- [Second-order cellular automaton](https://en.wikipedia.org/wiki/Second-order_cellular_automaton)
- [Cryptographic Properties of Second-Order Memory Elementary Cellular Automata](https://ieeexplore.ieee.org/document/4529417)
- [The Stone Cipher-192 (SC-192): A Metamorphic Cipher](https://d1wqtxts1xzle7.cloudfront.net/6257923/091101.pdf?response-content-disposition=inline%3B+filename%3DThe_Stone_Cipher_192_SC_192_A_Metamorphi.pdf&Expires=1643938338&Signature=Z5HpehmixBvYedndmkNJ6IQdIQLpCk7eLcD~eqX-UDlNSM03D60Zy9YW2CxCXydjIdlI-DVbKAdFhQWl~usaCJ-YSdUOJksu6qqkdhZdFRCXiqNw9H4288llkFoei9Pg50O8kAux-6-hZYmtBr0UwiRdW3Y-4~-75oHrYi4woJc8oJRX7I8qTB1V2b7Xzfu8P51fX9SXhEZcRP0PMyQHMh-Gd0dDexAKkjAAi3~1qtbfLtHaSaPX5DS5xEFieYsVUPrmF3-NP2~dibit2EC3rljetMfrSKKsWg7A5z9MOFGnTTATUNmeOqSg7wKFlrmu2QnRE-vlrchGJY9uhVc9KA__&Key-Pair-Id=APKAJLOHF5GGSLRBV4ZA)
