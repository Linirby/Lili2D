# Notes

There will be all current things I wanna remind without having to open github or
smth else.

## TASKS

### TODO

- OOP to ECS support: AnimatedSprite.
- OOP to ECS support: Tilemap & ChunkBatch.
- Keep global ECSRegistry in Game but add scene-dependent ECSRegistry.

### IN PROGRESS

- OOP to ECS support: Text.
  - append and render mesh (batch helper + dyn batch + render2d sys)

## KEEP FOR LATER

- GPUMesh::transferToGpu() -> replace every frame allocation, by a persistent buffer
  for the transfer buffer.
- DynamicBatch -> reserve vertex and index buffer capacity before appending batch
  items.
- BatchHelper::appendCircle() -> optimize circle rendering (precomputed unit circle
  LUT or SDF quad shader instead of recalculating 24 cos/sin per circle every frame).
