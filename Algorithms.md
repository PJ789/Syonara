# Algorithms

## Keyboard Scanning

### Stage 1 - Key Press Detection

The code scans the keyboard using two decade counters (which cycle through ten outputs following a clock pulse). Both counters are active, energising rows 0+10, then 1+11,.. 9+19.

A shift register is used to read the rows after each step.

If the shift register returns a non-zero value, or there was previously a value on either column n or n+1.... the iteration stops, and the second stage of scanning starts.

### Stage 2 - Key Press Validation

During the second stage, one of the decade counters is frozen, and the other decade counter iterates over a full cycle to confirm that the reading remains present (so verifying the reading on the first counter).

The decade counters are then brought back into sync, before the result is returned for decoding.
