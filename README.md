# RWLock-performance-test
Simple RWLock preformance test which comapres standart std::shared_mutex with folly::RWSpinLock. 
### Resulsts
|  | MSVC 2015 | GCC 7.2 |
|--|--|--|
| std::shared_mutex 5% write/95% read (calls per second) | 461003 | 15860
| std::shared_mutex 10% write/90% read (calls per second) | 461598 | 17146
| std::shared_mutex 20% write/80% read (calls per second) | 469828 | 15781
| folly::RWSpinLock 5% write/95% read (calls per second) | 458698 | 15552
| folly::RWSpinLock 10% write/90% read (calls per second) | 461857 | 16983
| folly::RWSpinLock 20% write/80% read (calls per second) | 458438 | 17075

Test results are not from same cpu.
