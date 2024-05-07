package cutehusky.AnimationComponent;

public class MolangHelper {
    private static native void nativeInitial(long uid);

    private static native void nativeRelease(long uid);

    private static native void nativeSetVariableByName(long uid, String varName, float value);

    private static native void nativeSetVariableByIndex(long uid, int varIndex, float value);

    private static native int nativeGetVariableIndex(String varName);

    private final long uid;

    public MolangHelper(long entity) {
        nativeInitial(entity);
        this.uid = entity;
    }

    public void Release() {
        nativeRelease(uid);
    }

    public void SetVariable(String varName, float value) {
        nativeSetVariableByName(this.uid, varName, value);
    }

    public void SetVariable(int varIndex, float value) {
        nativeSetVariableByIndex(this.uid, varIndex, value);
    }

    public int GetVariableIndex(String varName) {
        return nativeGetVariableIndex(varName);
    }
}