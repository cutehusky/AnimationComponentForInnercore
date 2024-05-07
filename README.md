# Animation Component For Innercore

A library for Innercore to control entity's animation via edit molang variable.

https://icmods.mineprogramming.org/mod?id=934

## Feature

- Only support set float molang variable in scripts.
- Class MolangHelper only work in client.
- Only support modifies float molang variable in scripts.
- Import

  ```typescript
  let AnimationComponet = null
  ModAPI.addAPICallback("AnimationComponet", function (api) {
  	AnimationComponet = api;
  });
  ```

## Dependency

KernelExtension:

https://github.com/DMHYT/KernelExtension

## Example

https://github.com/cutehusky/guncraft
