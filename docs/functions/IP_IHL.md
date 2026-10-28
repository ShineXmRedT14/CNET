```c
int IP_IHL(int data);
```

# Code
```c
int IP_IHL(int data) {
  return (20+data)/4;
}
```c
