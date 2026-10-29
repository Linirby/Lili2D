# Notes

There will be all current things I wanna remind without having to open github or
smth else.

## TASKS

### TODO

- OOP to ECS support: Tilemap & ChunkBatch.
- Keep global ECSRegistry in Game but add scene-dependent ECSRegistry.

### IN PROGRESS

- Update Parameters to create entities, to have a clearer vision on which
  components entities have.
- OOP to ECS support: AnimatedSprite.

## KEEP FOR LATER

- GPUMesh::transferToGpu() -> replace every frame allocation, by a persistent buffer
  for the transfer buffer.
- DynamicBatch -> reserve vertex and index buffer capacity before appending batch
  items.
- BatchHelper::appendCircle() -> optimize circle rendering (precomputed unit circle
  LUT or SDF quad shader instead of recalculating 24 cos/sin per circle every frame).
