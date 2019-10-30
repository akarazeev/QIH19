# Quantum Internet Hackathon 2019: OpenSSL Integration Challenge

In this challenge the task is to integrate QKD encryption into the OpenSSL
library. This will allow easy integration of many user applications with QKD
since many already use OpenSSL for the purposes of encryption. At the end of
the challenge, you will hopefully be able to demonstrate your integration by
running a browser session over a TLS connection encrypted with a QKD generated
key. The quantum network in this hackathon is simulated in the quantum network
simulator [SimulaQron](https://github.com/SoftwareQuTech/SimulaQron), but we
will be using the [CQC
API](https://softwarequtech.github.io/SimulaQron/html/CQCInterface.html) that
was designed to be supported on the first demonstration network that is
currently being built in the Netherlands.

## The challenge

The challenge is split up into two parts:
1. Integrate an abstract QKD API into OpenSSL
2. Implement QKD in software using the CQC API

The two parts can be done independently and they require different expertise so
you can chose which one is better aligned with your own interests.

## QKD API

The boundary between the two tasks is a high-level QKD API. We will not be
designing such an API from scratch, instead we will use one that
[ETSI](https://www.etsi.org/) has been working on. The fully documented API is
can be found in this repository
[QKD_Application_Interface.pdf](QKD_Application_Interface.pdf) or online
[URL](https://www.etsi.org/deliver/etsi_gs/QKD/001_099/004/01.01.01_60/gs_QKD004v010101p.pdf).

In the first task, the OpenSSL integration, the challenge is to integrate this
API into OpenSSL. We provide a mock implementation for testing purposes.

In the second task, the QKD implementation, the challenge is to implement a
full QKD scheme in software that uses the CQC API to interact with a simulated
network.

Once both tasks are complete, the mock implementation used in the first part
can be replaced with the full implementation from the second part so that we
can eventually run a browser session across the network.

Looking ahead, once the demo network is up and running, this OpenSSL
integration should then be able to run on top of the real thing by replacing
the simulated network with actual connections to the real hardware.

## OpenSSL integration

The first part is to integrate the QKD API with OpenSSL and demo it by running
a browser session (or some other user-level application) encrypted with QKD.

You can find the C definition of this API in [the `qkd_api.h`
header](qkd_api/qkd_api.h) and its documentation in its own
[README](qkd_api/README.md).

We have written up a small guide that should get you started with the OpenSSL
code base:

📖 [OpenSSL QKD integration manual](OpenSSL_QKD_integration.md)

### Mock

For testing purposes we also provide you with a mock implementation of this
API. This API does nothing quantum and has limited capabilities, but it should
be enough for you test the OpenSSL integration without having to wait for the
full software implementation from the second part. This mock implementation can
be found in [`qkd_api`](qkd_api) and its documentation is in
[`MOCK.md`](qkd_api/MOCK.md).

## QKD Implementation

An actual implementation of the QKD API is also needed. The goal here is to
implement the API defined in [`qkd_api`](qkd_api) such that the OpenSSL
integration can transparently switch over from the mock implementation to the
actual QKD implementation.

A high-level overview of what is necessary to produce a key using a QKD scheme
is illustrated [here](https://qkdsimulator.com/qkd_run_example.html). A more
detailed description with pseudocode can be found in the [Quantum Protocol
Zoo](https://wiki.veriqloud.fr/index.php?title=BB84_Quantum_Key_Distribution).
However, you may want to consider basing your implementation on the [E91
protocol](https://en.wikipedia.org/wiki/Quantum_key_distribution#E91_protocol:_Artur_Ekert_(1991))
instead of the BB84 protocol since the creation of entangled pairs is a more
fundamental operation than qubit transmission on the quantum internet.
