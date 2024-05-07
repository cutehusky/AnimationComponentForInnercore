/// <reference path="./core-engine.d.ts"/>

declare module AnimationComponent {
    export class MolangHelper extends java.lang.Object {
        constructor(entity: number);
        SetVariable(variableIndex: number, value: number): void;
        SetVariable(variableName: string, value: number): void;
        GetVariableIndex(variableName: string): number;
    }
    export function ReleaseEntity(uid: number): void;
    export function RegistryEntity(uid: number): void;
    export function SetMolangVariable(uid: number, name: string, value: string): void;
    export function getVersion(): number[];
}