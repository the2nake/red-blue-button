# red-blue-button

Everyone chooses either red or blue. If more than 50% of people choose blue, nothing happens. If more than 50% of people choose red, everyone who picked blue dies. Which is the better choice?

This repo contains code for a simulation with selfish, probabilistic agents playing through this scenario many times, slowly adjusting their inclinations based on the results of previous scenarios. TL;DR: blue is better. A sample output is shown below.

```txt
exp   0 survival | 1.00 1.00 1.00 1.00 1.00 1.00 | w = 0.485
exp  10 survival | 0.50 0.51 1.00 1.00 | w = 0.496
exp  20 survival | 0.50 1.00 1.00 1.00 1.00 | w = 0.489
exp  30 survival | 0.53 1.00 1.00 1.00 1.00 | w = 0.494
exp  40 survival | 0.50 1.00 1.00 1.00 1.00 | w = 0.492
exp  50 survival | 0.51 0.52 0.50 1.00 1.00 | w = 0.491
exp  60 survival | 0.51 1.00 1.00 1.00 1.00 | w = 0.479
exp  70 survival | 0.51 1.00 1.00 1.00 | w = 0.477
exp  80 survival | 0.51 0.51 0.51 0.50 0.52 0.52 ........................| w = 0.561
exp  90 survival | 1.00 1.00 1.00 1.00 | w = 0.484
---
  avg (weight): 0.512 (blue 0.0 -> 1.0 red)
stdev (weight): 0.051
25 / 100 worlds led to red (w > 0.5)

  avg (blue survival): 1.000
stdev (blue survival): 0.000
  avg (red survival): 0.597
stdev (red survival): 0.021
```
