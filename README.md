# QIH19

In case you want to change API you will need to redefine `typedef` for `context_t`.

---

### Documentation


```c
int QKD_OPEN(context_t* context);
```
Establishes a set of parameters that define the expected levels of key service.
- :param `context`:
- :return: status of operation. In case of errors returns `-1`, otherwise `0`

---

```c
int QKD_GET_KEY(context_t* context, uint32_t key_size, uint8_t* key_buffer);
```
Obtains the required amount of key material requested for this `context`. Each call returns the fixed amount (specified with `key_size`) of requested key stored in `key_buffer`.
- :param `context`:
- :param `key_size`: length of `key_buffer` as number of bytes
- :param `key_buffer`: place to store the requested key
- :return: status of operation. In case of errors returns `-1`, otherwise `0`

---

```c
int QKD_CLOSE(context_t* context);
```
Terminates the association established for this `context`.
- :param `context`:
- :return: status of operation. In case of errors returns `-1`, otherwise `0`

---
