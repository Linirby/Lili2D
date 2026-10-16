# Notes

There will be all current things I wanna remind without having to open github or
smth else.

## TASKS

### TODO

- Fix Render2DSystem ordering between different draw item types.
- OOP to ECS support: Text.
- OOP to ECS support: AnimatedSprite.
- OOP to ECS support: Tilemap & ChunkBatch.
- OOP to ECS support: UI Anchors & Pivots.
- Keep global ECSRegistry in Game but add scene-dependent ECSRegistry.

### IN PROGRESS

- Hide Render2DSystem in Game class.

### DONE

- OOP to ECS support: Line.
- OOP to ECS support: Circle.
- Optimize Render2DSystem with index sorting instead of struct.
- OOP to ECS support: Rect.
- Fix FPS loss with Nvidia on Linux when SpriteBatch::flush() is called in gameloop.
  (check dynamic_batch in examples/hello_sprite_batch)
- OOP to ECS support: Sprite.
- Separate SpriteBatch into StaticTextureBatch, DynamicTextureBatch and ChunkBatch.

## KEEP FOR LATER

- GPUMesh::transferToGpu() -> replace every frame allocation, by a persistent buffer
  for the transfer buffer.
