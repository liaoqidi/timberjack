package org.YiDongMM;

import com.secneo.mmb.Helper;
import android.app.Application;
import android.content.Context;

public class MyApplication extends Application {
	protected void attachBaseContext(Context ctx) {
		super.attachBaseContext(ctx);
		Helper.install(this);
	}
}