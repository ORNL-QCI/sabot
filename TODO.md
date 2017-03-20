* Thread-safety is very superficial and more robust solutions must be implemented, especially w.r.t. lists of protected items.
* ms_container needs to become a wrapper around std::map that provides move-semantics, key generation, and item thread-safety through reference counting.
* Requests should allow JSON and binary data through static dispatch from CRTP.
