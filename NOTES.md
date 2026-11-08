# Notes

There will be all current things I wanna remind without having to open github or
smth else.

## TASKS

### TODO

- OOP to ECS support: Tilemap & ChunkBatch.

### IN PROGRESS

- OOP to ECS support: AnimatedSprite.

## KEEP FOR LATER

- Simplify process to create an Animation (now: load atlas -> slice -> register)
- GPUMesh::transferToGpu() -> replace every frame allocation, by a persistent buffer
  for the transfer buffer.
- DynamicBatch -> reserve vertex and index buffer capacity before appending batch
  items.
- BatchHelper::appendCircle() -> optimize circle rendering (precomputed unit circle
  LUT or SDF quad shader instead of recalculating 24 cos/sin per circle every frame).
