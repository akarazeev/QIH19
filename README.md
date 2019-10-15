# QIH19

In case you want to change API you will need to redefine `typedef` for `context_t`.

---

### Documentation


```c
uint32_t QKD_OPEN(uint32_t destination, qos_t QoS, context_t* key_handle);
```
Establishes a set of parameters that define the expected levels of key service.
- :param `destination`:
- :param `QoS`:
- :param `key_handle`:
- :return: status of operation

---

```c
uint32_t QKD_CONNECT(context_t* key_handle, uint32_t timeout);
```
- :param `key_handle`:
- :param `timeout`:
- :return: status of operation

---

```c
uint32_t QKD_GET_KEY(context_t* key_handle, uint8_t* key_buffer);
```
Obtains the required amount of key material requested for this `key_handle`. Each call returns the fixed amount of requested key stored in `key_buffer`.
- :param `key_handle`:
- :param `key_buffer`: place to store the requested key
- :return: status of operation

---

```c
uint32_t QKD_CLOSE(context_t* key_handle);
```
Terminates the association established for this `key_handle`.
- :param `key_handle`:
- :return: status of operation

---
