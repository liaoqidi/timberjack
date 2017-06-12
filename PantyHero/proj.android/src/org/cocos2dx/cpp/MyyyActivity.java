package org.cocos2dx.cpp;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;

import com.pi.PantyHero.R;
import com.unicom.dcLoader.Utils;
import com.unicom.dcLoader.Utils.UnipayPayResultListener;

public class MyyyActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);//鍘绘帀鏍囬鏍�

		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);//鍘绘帀淇℃伅鏍�
		setContentView(R.layout.activity_main);
		
		handler.sendEmptyMessageDelayed(1000, 3300);
	}
	
	Handler handler = new Handler(){
		@Override
		public void handleMessage(Message msg) {
			if(msg.what == 1000){
				Intent intent = new Intent(MyyyActivity.this, Cocos2dxActivity.class);
				MyyyActivity.this.startActivity(intent);
				MyyyActivity.this.finish();
			}
		}
	};
	
}
