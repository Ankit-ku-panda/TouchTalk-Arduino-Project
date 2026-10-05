# Testing and evidence

## Included recording

The MP4 is the user's original Wokwi demonstration, copied without transcoding. The PNG preview is a frame at approximately 10 seconds. In that frame, the beginner sketch is visible, the LCD shows `Need the toilet`, and Serial Monitor lists Yes, No, Water, Food and Toilet messages. The GIF is a smaller animated derivative of the same recording. These are simulation visuals, not photographs of physical hardware.

## Checks performed for this package

- The beginner sketch was compiled as C++ using mocked Arduino, Serial and LCD interfaces.
- Assertions passed for all six phrase mappings, held-input single firing, Repeat, Clear, Repeat after Clear, simultaneous-input rejection and release-to-rearm.
- Repository media paths, diagram JSON, code-to-circuit pin mapping and archive integrity were checked.
- The original video copy was checked byte-for-byte.

The mock harness checks program logic, not real Arduino timing, debounce waveforms, buzzer sound or electrical behavior. A fresh Wokwi/AVR compilation was not performed during this packaging step. The included user recording supplies the visual Wokwi demonstration of the beginner version. Physical touch modules and accessibility/usability have not been tested.

## Try it yourself

1. Load the three Wokwi files following README.md and start the simulation.
2. Wait for `TouchTalk ready` and release all buttons.
3. Test 1–6 against the message table.
4. Hold an input: there should be one message until all inputs are released.
5. Repeat after a phrase: the same phrase should appear again.
6. Clear, then Repeat: `No message yet` should appear.
7. Activate two inputs together before either is accepted: an instruction to use one pad should appear. Release all before retrying.
8. Confirm the display and Serial Monitor match and check that browser audio permits the buzzer.

The two-sample 40 ms debounce is deliberately basic. It may miss brief presses or changes between samples and does not provide guaranteed rejection of all noisy signals.
