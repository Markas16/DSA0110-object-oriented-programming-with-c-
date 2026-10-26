# UML Architecture & Behavioral Diagrams

**System:** City Metro Smart-Card Fare Logic System  
**Student:** Markas (Register No: 192421250)  

---

## 1. Class Diagram (Structural Model)

```mermaid
classDiagram
    direction TB

    class Zone {
        <<enumeration>>
        +Inner = 0
        +Middle = 1
        +Outer = 2
        +Airport = 3
        +Unknown = 255
    }

    class ConcessionType {
        <<enumeration>>
        +None
        +Student
        +Senior
    }

    class JourneyRecord {
        +uint64_t journeyId
        +Zone destinationZone
        +int travelHour
        +bool isWeekend
        +int64_t farePaise
        +bool authorized
        +int64_t balanceAfterPaise
        +string notes
    }

    class FareEngine {
        <<service>>
        +int64_t BASE_INNER = 1000
        +int64_t BASE_MIDDLE = 2000
        +int64_t BASE_OUTER = 3000
        +int64_t BASE_AIRPORT = 6000
        +int64_t MIN_BALANCE_PAISE = 5000
        +isPeakHour(int hour) bool
        +calculateFare(Zone z, int hour, bool isWeekend) int64_t
        +formatRupees(int64_t paise) string
    }

    class MonthlyPass {
        -bool active_
        -Zone zone1_
        -Zone zone2_
        -uint64_t passExpiryTimestamp_
        +MonthlyPass()
        +MonthlyPass(Zone z1, Zone z2, uint64_t expiry)
        +isActive() bool
        +coversZone(Zone z) bool
        +getDescription() string
    }

    class SmartCard {
        -uint64_t cardId_
        -int64_t balancePaise_
        -bool active_
        -vector~JourneyRecord~ journeyHistory_
        -MonthlyPass pass_
        -ConcessionType concession_
        +SmartCard(uint64_t cardId, int64_t initialBalancePaise)
        +getCardId() uint64_t
        +getBalancePaise() int64_t
        +getBalanceRupees() double
        +canTravel(int64_t farePaise) bool
        +processTrip(Zone zone, int hour, bool isWeekend) bool
        +recharge(int64_t amountPaise) bool
        +attachMonthlyPass(MonthlyPass pass) void
        +printReport() void
    }

    SmartCard "1" *-- "*" JourneyRecord : maintains history
    SmartCard "1" o-- "1" MonthlyPass : optional subscription
    SmartCard ..> Zone : traverses
    SmartCard ..> FareEngine : requests pricing
    MonthlyPass ..> Zone : covers
```

---

## 2. Sequence Diagram: Tap-and-Go Fare Processing

```mermaid
sequenceDiagram
    autonumber
    actor Commuter
    participant Turnstile as Automated Gate
    participant SmartCard as SmartCard Object
    participant FareEngine as FareEngine Service

    Commuter->>Turnstile: Tap Smart-Card at Exit Turnstile
    Turnstile->>SmartCard: Query Card ID & Active Status
    Turnstile->>FareEngine: calculateFare(Zone, Hour, isWeekend)
    FareEngine-->>Turnstile: Return Calculated Fare (paise)
    Turnstile->>SmartCard: canTravel(farePaise)
    Note over SmartCard: Invariant check:<br/>(balance - fare) >= 5000 paise
    alt Solvency Check Passes (Balance - Fare >= 5000)
        SmartCard->>SmartCard: balancePaise -= farePaise
        SmartCard->>SmartCard: append JourneyRecord(Authorized = true)
        SmartCard-->>Turnstile: Transaction Approved
        Turnstile-->>Commuter: Gates OPEN & Display Balance
    else Solvency Check Fails (Balance - Fare < 5000)
        SmartCard->>SmartCard: append JourneyRecord(Authorized = false)
        SmartCard-->>Turnstile: Transaction Denied (Low Balance)
        Turnstile-->>Commuter: Gates LOCKED: Please Recharge (Min Rs 50 required)
    end
```

---

## 3. State Machine Diagram: Smart-Card Balance Lifecycle

```mermaid
stateDiagram-v2
    [*] --> Initialized: Card Issued (Default 10,000 paise / Rs 100)
    
    state Initialized {
        [*] --> Solvent
        Solvent --> Solvent: Trip Approved (balance - fare >= 5000)
        Solvent --> NearMinimum: balance - fare == 5000
        NearMinimum --> Solvent: Recharge (amount > 0)
        NearMinimum --> DeficitBlocked: Trip Attempted (balance - fare < 5000)
        DeficitBlocked --> Solvent: Successful Recharge (balance >= 5000)
    }

    DeficitBlocked --> [*]: Card Deactivated
```
