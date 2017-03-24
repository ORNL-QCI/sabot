# Sabot
## Examples

These examples show how one may interact with Sabot. Although the output of each example is interesting, take a look at the source code for each example.

To run these examples, first start Sabot:
```
$ sabot -e ipc:///tmp/sabot
```

### `qrng.py`
Create 5000 qubits and prepare each in the superposition state 1/sqrt(2)(|0> + |1>) before measuring them. Each measurement yields a 0 or a 1, and these results are averaged to yield a number close to 1/2.

### `sdc.py`
"Communicate" 2 bits of classical information through the "transmission" of 1 qubit. Two bits are randomly encoded and after noiseless transmission are successfully decoded. The overall circuit description is printed as well.

### `teleportation.py`
"Communicate" 1 qubit through the "transmission" of 2 bits of classical information. This example shows how a controlled gate is performed classically, as the logic for the controlled-x and controlled-y gates is in the python file, not the quantum circuit description. The overall circuit description is printed as well.
