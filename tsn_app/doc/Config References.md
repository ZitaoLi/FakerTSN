### General Items

#### Log Items

| key       | value                 | comment                                                      |
| --------- | --------------------- | ------------------------------------------------------------ |
| log.level | info/debug/warn/error | log level                                                    |
| log.path  |                       | related path of  log files                                   |
| log.reset | yes/no                | reset log files                                              |
| log.mode  | std/file              | log mode<br />std: output into stdout<br />file: output into log file |

#### Context Items

| key                                         | value                          | comment                                                      |
| ------------------------------------------- | ------------------------------ | ------------------------------------------------------------ |
| profile.path                                |                                | root path of config directory                                |
| workMode                                    | switch/host/configurator       | work mode                                                    |
| deviceName                                  |                                | device name                                                  |
| enabledTSN                                  | yes/no                         | enableing TSN functions                                      |
| switch.port.class                           | FlowApp/TSNController/DataPort | instance of port interface<br />FlowApp: producing and consuming flows, working on host mode<br />TSNController: controlling TSN network, working on configurator mode<br />DataPort: receiving and send TSN PDU, working on switch mode |
| switch.port.infs                            |                                | interface name of data port                                  |
| switch.port.cons                            |                                | interface name of control port                               |
| switch.port.per                             |                                | packet loss rate                                             |
| switch.queue.number                         | 1/2/3/4/5/6/7/8                | no. of queues                                                |
| switch.queue.class                          |                                | instance of queue interface                                  |
| switch.queue.capacity                       |                                | capacity of queue                                            |
| switch.queue.transmissionSelectionAlgorithm |                                |                                                              |
| switch.gcl                                  |                                | instance of gal interface                                    |
| sizeUnit                                    | B/K/M/G                        | data size unit                                               |

#### Time items

| key           | value      | comment                         |
| ------------- | ---------- | ------------------------------- |
| timeUnit      | ns/us/ms/s | time unit                       |
| timePrecision |            | time precision, time unit is ns |

#### Statistic Items

| key       | value | comment                    |
| --------- | ----- | -------------------------- |
| data.path |       | filename of statistic file |

