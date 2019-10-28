Please add a README document with a link to https://www.etsi.org/deliver/etsi_gs/QKD/001_099/004/01.01.01_60/gs_QKD004v010101p.pdf

Might also be worth adding the PDF to this repo

Also please write a brief summary of what each file is, and a high-level overview of how it works. You should make it clear that i) this is a mock API, ii) in it's current form it is only suitable for local testing only (explain the client/server assignment process), iii) it can be extended, iv) make it clear that client.c and server.c are just examples

Also make sure to note all limitations beyond being only able to use this mock locally such as:
- can only run one session at a time on the same key_handle
