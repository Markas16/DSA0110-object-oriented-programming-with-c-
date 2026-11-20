# Task 11: Evidence-Based Final System Architecture & Design Defence

## 1. Executive Summary & Design Recommendations

Based on empirical data gathered across Tasks 1–10, the following architectural decisions are defended for the City Metro Smart-Card Fare Collection System:

| Architectural Dimension | Final Selection | Direct Evidence & Justification |
| :--- | :--- | :--- |
| **Money Representation** | `int64_t` in **paise** | IEEE 754 floating-point numbers (`double`) cannot represent decimal fractions like 0.10 accurately, accumulating drift in financial audits (Task 3). `int32_t` overflows at ~2.14 crore paise (Task 10 benchmark), whereas `int64_t` safely handles collections up to ₹9.22 × 10¹⁴ without overflow. |
| **Temporal Data Types** | `int` (0–23) | Native machine word arithmetic prevents signed/unsigned promotion bugs (Task 4) while maintaining strict validation $0 \le \text{hour} \le 23$. |
| **Zone Model** | `enum class Zone : uint8_t` | Strong typing prevents implicit conversions to raw integers or cross-zone misuse (Task 2). Memory footprint is just 1 byte per instance. |
| **Constants & Scope** | `inline constexpr` | Eliminates macro hazards (side-effects, text expansion, missing debug symbols). Guaranteed compile-time evaluation with zero runtime overhead (Task 2). |
| **State Encapsulation** | `SmartCard` with `private` balance | Guarantees business invariants: cards can never be deducted below 5000 paise (₹50.00). Prevents direct mutation bugs demonstrated in Task 7. |
| **Fare Engine Design** | Decoupled Stateless Service | Decouples pricing policies (peak surcharges, weekend discounts, monthly passes) from commuter card state, achieving high modularity and unit testability. |

---

## 2. Configurable Fare Table vs. Compiled-in `constexpr` Values

### 2.1 Comparative Analysis

| Feature | Static `constexpr` Table | Configurable Startup Fare Table |
| :--- | :--- | :--- |
| **Initialization Cost** | $\mathcal{O}(1)$ (Zero runtime cost, baked into binary) | $\mathcal{O}(z)$ startup time to parse config file for $z$ zones |
| **Memory Allocation** | $\mathcal{O}(1)$ (Stored in text/rodata segment, zero heap) | $\mathcal{O}(z)$ heap memory for dynamic lookups (hash map / vector) |
| **Tariff Modification** | Requires full recompilation and deployment | Modified in JSON/CSV config file without touching executable binary |
| **Type Safety** | Verified by compiler at compile-time | Requires runtime schema validation, error handling, and fallback logic |
| **Branching Performance** | Direct inlining, compiler optimization | Indirect table pointer lookup, negligible cache miss penalty |

### 2.2 Recommendation
- **For Embedded / Fast Turnstiles**: Use the `constexpr` approach for low latency, zero memory allocations, and hard real-time guarantees.
- **For Municipal Metro Production Systems**: Use the **configurable startup table** with compile-time defaults as a safe fallback. Loading $z$ zones at startup requires trivial $\mathcal{O}(z)$ time (< 1 ms for dozens of zones) while allowing municipal tariff revisions without system re-certification.

---

## 3. Scalability, Bottlenecks, and Limitations

1. **Throughput Scalability**:
   - The in-memory benchmark (Task 10) demonstrated over 40–60 million trips processed per second on standard hardware with integer arithmetic.
   - The primary real-world bottleneck is not CPU arithmetic, but I/O persistence (database commits, contactless RFID read/write latency).
2. **Current System Boundaries**:
   - Flat zone-to-zone base tariffs (origin-destination matrix is simplified into destination zonal tiers).
   - Time intervals are currently evaluated on hourly integers rather than minute-level granularity.
3. **Future Extensibility**:
   - **Origin-Destination Pricing Matrix**: Replacing 1D zone fare with a 2D matrix $\text{fareTable}[z_{origin}][z_{dest}]$.
   - **Capping / Daily Maximums**: Automatically limiting fares once daily spend hits a fixed ceiling.
   - **Concurrency**: Thread-safe card transactions using atomic operations (`std::atomic<int64_t>`) or mutex locks for multi-threaded gate turnstiles.
