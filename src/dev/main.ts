const MolangHelper = WRAP_JAVA("cutehusky.AnimationComponent.MolangHelper");
/*
// test
let a = 0;
let molang = null;
let varIndex = 0;
Callback.addCallback("ServerPlayerTick", function (id: number) {
    if (molang == null) {
        molang = new MolangHelper(id);
        varIndex = molang.GetVariableIndex("variable.abc");
    }
    //molang.SetVariable("variable.abc", a + 12345.0);
    molang.SetVariable(varIndex, a + 12345.0);
    a++;
    a %= 10;
});
*/

/*
// test
let a = 0;
Callback.addCallback("ServerPlayerTick", function (uid: number) {
    Network.getClientForPlayer(uid).send("AnimationComponet.setVariable", {
        uid: uid,
        name: "variable.abc",
        value: a + 12345.0
    });
    a++;
    a %= 10;
});
*/

const MolangHelperContainer = {}

Network.addClientPacket("AnimationComponet.releaseEntity",
    function (data: { uid: number }): void {
        if (MolangHelperContainer.hasOwnProperty(data.uid)) {
            MolangHelperContainer[data.uid].Release();
            delete (MolangHelperContainer[data.uid]);
        }
    });

Network.addClientPacket("AnimationComponet.registerEntity",
    function (data: { uid: number }): void {
        MolangHelperContainer[data.uid] = new MolangHelper(data.uid);
    });

Network.addClientPacket("AnimationComponet.setVariable",
    function (data: { uid: number, name: string, value: number }): void {
        if (!MolangHelperContainer.hasOwnProperty(data.uid))
            MolangHelperContainer[data.uid] = new MolangHelper(data.uid);
        MolangHelperContainer[data.uid].SetVariable(data.name, data.value);
    });

ModAPI.registerAPI("AnimationComponet", {
    MolangHelper: MolangHelper,
    ReleaseEntity: (uid: number) => {
        Network.sendToAllClients("AnimationComponet.releaseEntity", { uid: uid });
    },
    RegistryEntity: (uid: number) => {
        Network.sendToAllClients("AnimationComponet.registerEntity", { uid: uid });
    },
    SetMolangVariable: (uid: number, name: string, value: string) => {
        Network.sendToAllClients("AnimationComponet.setVariable", { uid: uid, name: name, value: value })
    },
    getVersion: () => { return [1, 0, 0] }
});