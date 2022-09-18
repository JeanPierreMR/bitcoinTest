# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libcscoin_cli*         | RPC client functionality used by *cscoin-cli* executable |
| *libcscoin_common*      | Home for common functionality shared by different executables and libraries. Similar to *libcscoin_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libcscoin_consensus*   | Stable, backwards-compatible consensus functionality used by *libcscoin_node* and *libcscoin_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *libcscoinconsensus*    | Shared library build of static *libcscoin_consensus* library |
| *libcscoin_kernel*      | Consensus engine and support library used for validation by *libcscoin_node* and also exposed as a [shared library](../shared-libraries.md). |
| *libcscoinqt*           | GUI functionality used by *cscoin-qt* and *cscoin-gui* executables |
| *libcscoin_ipc*         | IPC functionality used by *cscoin-node*, *cscoin-wallet*, *cscoin-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *libcscoin_node*        | P2P and RPC server functionality used by *cscoind* and *cscoin-qt* executables. |
| *libcscoin_util*        | Home for common functionality shared by different executables and libraries. Similar to *libcscoin_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libcscoin_wallet*      | Wallet functionality used by *cscoind* and *cscoin-wallet* executables. |
| *libcscoin_wallet_tool* | Lower-level wallet functionality used by *cscoin-wallet* executable. |
| *libcscoin_zmq*         | [ZeroMQ](../zmq.md) functionality used by *cscoind* and *cscoin-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *libcscoin_consensus* and *libcscoin_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`libcscoin_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libcscoin_node* code lives in `src/node/` in the `node::` namespace
  - *libcscoin_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libcscoin_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libcscoin_util* code lives in `src/util/` in the `util::` namespace
  - *libcscoin_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "linear" }}}%%

graph TD;

cscoin-cli[cscoin-cli]-->libcscoin_cli;

cscoind[cscoind]-->libcscoin_node;
cscoind[cscoind]-->libcscoin_wallet;

cscoin-qt[cscoin-qt]-->libcscoin_node;
cscoin-qt[cscoin-qt]-->libcscoinqt;
cscoin-qt[cscoin-qt]-->libcscoin_wallet;

cscoin-wallet[cscoin-wallet]-->libcscoin_wallet;
cscoin-wallet[cscoin-wallet]-->libcscoin_wallet_tool;

libcscoin_cli-->libcscoin_common;
libcscoin_cli-->libcscoin_util;

libcscoin_common-->libcscoin_util;
libcscoin_common-->libcscoin_consensus;

libcscoin_kernel-->libcscoin_consensus;
libcscoin_kernel-->libcscoin_util;

libcscoin_node-->libcscoin_common;
libcscoin_node-->libcscoin_consensus;
libcscoin_node-->libcscoin_kernel;
libcscoin_node-->libcscoin_util;

libcscoinqt-->libcscoin_common;
libcscoinqt-->libcscoin_util;

libcscoin_wallet-->libcscoin_common;
libcscoin_wallet-->libcscoin_util;

libcscoin_wallet_tool-->libcscoin_util;
libcscoin_wallet_tool-->libcscoin_wallet;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class cscoin-qt,cscoind,cscoin-cli,cscoin-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libcscoin_wallet* and *libcscoin_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code still is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libcscoin_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libcscoin_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *libcscoin_common* should serve a similar function as *libcscoin_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *libcscoin_util* and *libcscoin_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for cscoin-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *libcscoin_kernel* is not supposed to depend on *libcscoin_common*, only *libcscoin_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *libcscoin_kernel* should only depend on *libcscoin_util* and *libcscoin_consensus*.

- The only thing that should depend on *libcscoin_kernel* internally should be *libcscoin_node*. GUI and wallet libraries *libcscoinqt* and *libcscoin_wallet* in particular should not depend on *libcscoin_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *libcscoin_consensus*, *libcscoin_common*, and *libcscoin_util*, instead of *libcscoin_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libcscoinqt*, *libcscoin_node*, *libcscoin_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *libcscoin_node* to *libcscoin_kernel* as part of [The libcscoinkernel Project #24303](https://github.com/cscoin/cscoin/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/cscoin/cscoin/issues/15732)
