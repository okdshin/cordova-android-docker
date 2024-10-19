package com.rs.my.testapp.plugin;

import org.apache.cordova.CallbackContext;
import org.apache.cordova.CordovaPlugin;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;


public class NdkEcho extends CordovaPlugin {
    public static native String echo(String input);

    // load library
    static {
        System.loadLibrary("echo");
    }

    /**
     * Executes the request and returns PluginResult.
     *
     * @param action            The action to execute.
     * @param args              JSONArry of arguments for the plugin.
     * @param callbackContext   The callback id used when calling back into JavaScript.
     * @return                  True if the action was valid, false if not.
     */
    public boolean execute(String action, JSONArray args, CallbackContext callbackContext) throws JSONException {
        if (action.equals("Echo")) {
            String message = args.getString(0);
            JSONObject r = new JSONObject();
            r.put("message", echo(message));
            //r.put("message", message);
            callbackContext.success(r);
        }
        else {
            return false;
        }
        return true;
    }
}
