Shared Libraries
================

## cscoinconsensus

The purpose of this library is to make the verification functionality that is critical to CScoin's consensus available to other applications, e.g. to language bindings.

### API

The interface is defined in the C header `cscoinconsensus.h` located in `src/script/cscoinconsensus.h`.

#### Version

`cscoinconsensus_version` returns an `unsigned int` with the API version *(currently `1`)*.

#### Script Validation

`cscoinconsensus_verify_script` returns an `int` with the status of the verification. It will be `1` if the input script correctly spends the previous output `scriptPubKey`.

##### Parameters
- `const unsigned char *scriptPubKey` - The previous output script that encumbers spending.
- `unsigned int scriptPubKeyLen` - The number of bytes for the `scriptPubKey`.
- `const unsigned char *txTo` - The transaction with the input that is spending the previous output.
- `unsigned int txToLen` - The number of bytes for the `txTo`.
- `unsigned int nIn` - The index of the input in `txTo` that spends the `scriptPubKey`.
- `unsigned int flags` - The script validation flags *(see below)*.
- `cscoinconsensus_error* err` - Will have the error/success code for the operation *(see below)*.

##### Script Flags
- `cscoinconsensus_SCRIPT_FLAGS_VERIFY_NONE`
- `cscoinconsensus_SCRIPT_FLAGS_VERIFY_P2SH` - Evaluate P2SH ([BIP16](https://github.com/cscoin/bips/blob/master/bip-0016.mediawiki)) subscripts
- `cscoinconsensus_SCRIPT_FLAGS_VERIFY_DERSIG` - Enforce strict DER ([BIP66](https://github.com/cscoin/bips/blob/master/bip-0066.mediawiki)) compliance
- `cscoinconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY` - Enforce NULLDUMMY ([BIP147](https://github.com/cscoin/bips/blob/master/bip-0147.mediawiki))
- `cscoinconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY` - Enable CHECKLOCKTIMEVERIFY ([BIP65](https://github.com/cscoin/bips/blob/master/bip-0065.mediawiki))
- `cscoinconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY` - Enable CHECKSEQUENCEVERIFY ([BIP112](https://github.com/cscoin/bips/blob/master/bip-0112.mediawiki))
- `cscoinconsensus_SCRIPT_FLAGS_VERIFY_WITNESS` - Enable WITNESS ([BIP141](https://github.com/cscoin/bips/blob/master/bip-0141.mediawiki))

##### Errors
- `cscoinconsensus_ERR_OK` - No errors with input parameters *(see the return value of `cscoinconsensus_verify_script` for the verification status)*
- `cscoinconsensus_ERR_TX_INDEX` - An invalid index for `txTo`
- `cscoinconsensus_ERR_TX_SIZE_MISMATCH` - `txToLen` did not match with the size of `txTo`
- `cscoinconsensus_ERR_DESERIALIZE` - An error deserializing `txTo`
- `cscoinconsensus_ERR_AMOUNT_REQUIRED` - Input amount is required if WITNESS is used
- `cscoinconsensus_ERR_INVALID_FLAGS` - Script verification `flags` are invalid (i.e. not part of the libconsensus interface)

### Example Implementations
- [NCScoin](https://github.com/MetacoSA/NCScoin/blob/5e1055cd7c4186dee4227c344af8892aea54faec/NCScoin/Script.cs#L979-#L1031) (.NET Bindings)
- [node-libcscoinconsensus](https://github.com/bitpay/node-libcscoinconsensus) (Node.js Bindings)
- [java-libcscoinconsensus](https://github.com/dexX7/java-libcscoinconsensus) (Java Bindings)
- [cscoinconsensus-php](https://github.com/Bit-Wasp/cscoinconsensus-php) (PHP Bindings)
