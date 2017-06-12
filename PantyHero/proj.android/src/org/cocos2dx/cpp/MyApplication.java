package org.cocos2dx.cpp;

import android.app.Application;
import com.unicom.dcLoader.Utils;
import com.unicom.dcLoader.Utils.UnipayPayResultListener;

public class MyApplication extends com.unicom.shield.UnicomApplicationWrapper{
	
	public void onCreate(){

		super.onCreate();

		try{
		Utils.getInstances().initSDK(this,new UnipayPayResultListener() {

			public void PayResult (String code, int flag, int flag1, String error) { 
				
			}
		});
		}catch(Exception e){
			e.printStackTrace();
		}

	}

}
