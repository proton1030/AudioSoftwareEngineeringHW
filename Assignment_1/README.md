# 2018-MUSI6106-Assignment1-Ring Buffer



Test case 1: testing get function with large offset 

```
output: 0	 expected: 0
```

Test case 2: testing get function with negative offset:

```
output: 0	 expected: 0
```

Test case 3: testing put and get function:

```
output: 1111.11	 expected: 1111.11
```

Test case 4: testing put with changing WriteIdx (+1), and using get function with offset and original ReadIdx:

```
offset=0 output: 1111.11	 expected: 1111.11

offset=1 output: 2222.22	 expected: 2222.22
```

Test case 5: testing put with changing WriteIdx (+1) and ReadIdx (-1), and using get function with offset:

```
offset=1 output: 1111.11	 expected: 1111.11
offset=2 output: 2222.22	 expected: 2222.22
offset=0 output: 0	 expected: 0
```
