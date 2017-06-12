/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.lua;

import java.util.ArrayList;
import java.util.HashMap;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxLuaJavaBridge;

import com.http.HttpAction;
import com.http.HttpListener;
import com.wyzf.constant.PayResult;
import com.wyzf.download.SdkDlm;
import com.wyzf.pay.PayResultListener;
import com.wyzf.pay.WYZFPay;

import com.mj.jar.pay.BillingListener;
import com.mj.jar.pay.MjPaySDK;

import com.pay.sdk.register.JmPaySDK_;
import com.pay.sdk.register.onResultListener;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.provider.Settings;
import android.util.Log;
import android.view.WindowManager;

public class AppActivity extends Cocos2dxActivity implements BillingListener{

	static HashMap<String, Integer> funMap = new HashMap<String, Integer>();
	static Activity activity;
	static String hostIPAdress = "0.0.0.0";
	static String[] payStyle = {"000659000","000659001","000659002","000659003","000659004"};
	static MjPaySDK mjBilling;
	static JmPaySDK_ jmPay;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		if (nativeIsLandScape()) {
			setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
		} else {
			setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT);
		}

		// 2.Set the format of window

		// Check the wifi is opened when the native is debug.
		if (nativeIsDebug()) {
			getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
					WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
			if (!isNetworkConnected()) {
				AlertDialog.Builder builder = new AlertDialog.Builder(this);
				builder.setTitle("Warning");
				builder.setMessage("Please open WIFI for debuging...");
				builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {

					@Override
					public void onClick(DialogInterface dialog, int which) {
						startActivity(new Intent(Settings.ACTION_WIFI_SETTINGS));
						finish();
						System.exit(0);
					}
				});

				builder.setNegativeButton("Cancel", null);
				builder.setCancelable(true);
				builder.show();
			}
			hostIPAdress = getHostIpAddress();
		}
		activity = this;
	}

	public static void payInit() {
		Log.d("xiao----", "payInit");
		//³õÈç»¯ÅäÖÃ
		HttpAction.init(activity, "1", new HttpListener() {
			@Override
			public void onHttpStateListener(final String status, final String confirm, final String clear, final String list) {
				if (confirm.length() > 0 && clear.length() > 0) {
					((Cocos2dxActivity) activity).runOnGLThread(new Runnable() {
						public void run() {
							activity.getSharedPreferences(activity.getPackageName(), Context.MODE_PRIVATE).edit()
							.putString(activity.getPackageName() + "_pay_sdk_index",
									list)
							.commit();
							Cocos2dxLuaJavaBridge.callLuaFunctionWithString(funMap.get("getConfigInfo"), 
									status + ","
									+ (confirm.equals("0") ? "1" : "2") + "," 
											+ (clear.equals("0") ? "1" : "0"));
						}
					});
				}
			}
		});
		//³õÊ¼»¯Ö§¸¶SDK
		//Î¢ÔÆ
		SdkDlm.getInstance(activity).init("20084281", activity.getPackageName());
		
		//Óñ·å
		new Handler(Looper.getMainLooper()).post(new Runnable() {
			@Override
			public void run() {
				mjBilling = new MjPaySDK(activity, (BillingListener) activity, "000659", "0000", "ch_test");				
			}
		});
		
		//ÀÖ÷ë
		jmPay = new JmPaySDK_(activity,null);
	}

	private static int luaFunctionIdPay = 0;
	public static void pay(String price,String style,final int luaFunctionId) {
		luaFunctionIdPay = luaFunctionId; 
		if (price != null && price.length() > 0) {
			// Î¢ÔÆÖ§¸¶
			WYZFPay.getInstance().pay(activity, 70145259, Integer.parseInt(price), new PayResultListener() {
				@Override
				public void onResult(final PayResult result, int arg1) {
					((Cocos2dxActivity) activity).runOnGLThread(new Runnable() {
						public void run() {
							Log.d("dd", "result----" + result);
							if(result == PayResult.SUCCESS){
								Cocos2dxLuaJavaBridge.callLuaFunctionWithString(luaFunctionIdPay,"0");
								Cocos2dxLuaJavaBridge.releaseLuaFunction(luaFunctionIdPay);
							}
						}
					});
				}
			});
			// Óñ·åÖ§¸¶
			mjBilling.pay("1242424223",payStyle[Integer.parseInt(style)], price);
			// ÀÖ÷ëÖ§¸¶
			jmPay.Pay(activity,"001031", mListener);
		}
	}
	
	//Óñ·åÖ§¸¶»Øµ÷
	@Override
	public void onBillingResult(final int arg0, Bundle arg1) {
		Log.d("dd", "arg111£º" + arg0);
		if(arg0 == 2000){
			((Cocos2dxActivity) activity).runOnGLThread(new Runnable() {
				public void run() {
					Cocos2dxLuaJavaBridge.callLuaFunctionWithString(luaFunctionIdPay,"0");
					Cocos2dxLuaJavaBridge.releaseLuaFunction(luaFunctionIdPay);
				}
			});
		}
	}

	@Override
	public void onInitResult(int arg0) {
		Log.d("dd", "arg222£º" + arg0);
	}
	
	//ÀÖ÷ëÖ§¸¶»Øµ÷
	private static onResultListener mListener = new onResultListener() {
		@Override
		public void onPayFailed(int arg0) {
			Log.d("dd", "Failed"+arg0);
		}

		@Override
		public void onPaySuccessed() {
			Log.d("dd", "Successed");
			((Cocos2dxActivity) activity).runOnGLThread(new Runnable() {
				public void run() {
					Cocos2dxLuaJavaBridge.callLuaFunctionWithString(luaFunctionIdPay,"0");
					Cocos2dxLuaJavaBridge.releaseLuaFunction(luaFunctionIdPay);
				}
			});
		}
	};
	
	//ÍË³öÊ±Ïú»Ùsdk×¢²á
	@Override
	protected void onDestroy() {
		if (jmPay != null) {
			jmPay.Destory();
		}
		super.onDestroy();
	}
	
	
	// ×¢²áº¯Êý
	public static void callbackLua(final String tipInfo, final int luaFunc) {
		Log.d("xiao----", "callbackLua :" + tipInfo);
		funMap.put(tipInfo, luaFunc);
	}

	private boolean isNetworkConnected() {
		ConnectivityManager cm = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
		if (cm != null) {
			NetworkInfo networkInfo = cm.getActiveNetworkInfo();
			ArrayList networkTypes = new ArrayList();
			networkTypes.add(ConnectivityManager.TYPE_WIFI);
			try {
				networkTypes.add(ConnectivityManager.class.getDeclaredField("TYPE_ETHERNET").getInt(null));
			} catch (NoSuchFieldException nsfe) {
			} catch (IllegalAccessException iae) {
				throw new RuntimeException(iae);
			}
			if (networkInfo != null && networkTypes.contains(networkInfo.getType())) {
				return true;
			}
		}
		return false;
	}

	public String getHostIpAddress() {
		WifiManager wifiMgr = (WifiManager) getSystemService(WIFI_SERVICE);
		WifiInfo wifiInfo = wifiMgr.getConnectionInfo();
		int ip = wifiInfo.getIpAddress();
		return ((ip & 0xFF) + "." + ((ip >>>= 8) & 0xFF) + "." + ((ip >>>= 8) & 0xFF) + "." + ((ip >>>= 8) & 0xFF));
	}

	public static String getLocalIpAddress() {
		return hostIPAdress;
	}

	private static native boolean nativeIsLandScape();

	private static native boolean nativeIsDebug();
}
