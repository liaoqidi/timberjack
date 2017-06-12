package org.YiDongMM;

import org.cocos2dx.cpp.Cocos2dxActivity;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Handler;
import android.os.Message;

import com.pi.PantyHero.R;

public class IAPHandler extends Handler {

	public static final int INIT_FINISH = 10000;
	public static final int BILL_FINISH = 10001;
	public static final int QUERY_FINISH = 10002;
	public static final int UNSUB_FINISH = 10003;

	private Cocos2dxActivity context;

	public IAPHandler(Activity context)
	{
		this.context = (Cocos2dxActivity) context;
	}

	@Override
	public void handleMessage(Message msg)
	{
		super.handleMessage(msg);
		int what = msg.what;
		switch (what)
		{
		case INIT_FINISH:
			initShow((String) msg.obj);
			context.dismissProgressDialog();
			break;
		default:
			break;
		}
		//showDialog("�����ܰ�", msg.toString());
	}

	private void initShow(String msg)
	{
	//	Toast.makeText(context, "" + msg, Toast.LENGTH_LONG).show();//surx
	}

	public void showDialog(String title, String msg)
	{
		AlertDialog.Builder builder = new AlertDialog.Builder(context);
		builder.setTitle(title);
		builder.setIcon(context.getResources().getDrawable(R.drawable.icon));
		builder.setMessage((msg == null) ? "Undefined error" : msg);
		builder.setPositiveButton("OK", new DialogInterface.OnClickListener()
		{
			public void onClick(DialogInterface dialog, int whichButton)
			{
				dialog.dismiss();
			}
		});
		builder.create().show();
	}

}
