# Battery Monitor

This project is a battery monitor that reports a battery's current voltage on a regular interval to a remote API that can capture trends, send alerts, etc. It contains two components:

| Component | Purpose                          |
| --------- | -------------------------------- |
| Sender    | An Arduino-based voltage monitor |
| Receiver  | TBD                              |

## Operation

### Startup

When the unit is powered on, it will attempt to establish a wifi connection.

| Sequence            | Meaning                    |
| ------------------- | -------------------------- |
| Blue, Blue, Blue    | Attempting wifi connection |
| Green, Green, Green | Connected to wifi          |
| Red, Red, Red       | Failed to connect to wifi  |

### Monitoring

After startup, the unit will sample the battery voltage on an interval and attempt to report the voltage to the remote endpoint.

| Sequence       | Meaning                       |
| -------------- | ----------------------------- |
| Green, Green   | >80% voltage                  |
| Yellow, Yellow | 60-80% voltage                |
| Red, Red       | <60% voltage                  |
| Blue, Green    | Successfully reported voltage |
| Blue, Red      | Failed to report voltage      |
