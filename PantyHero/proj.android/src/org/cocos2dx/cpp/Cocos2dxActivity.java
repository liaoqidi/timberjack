package org.cocos2dx.cpp;

import mm.purchasesdk.Purchase;

import org.YiDongMM.IAPHandler;
import org.YiDongMM.IAPListener;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.NativeActivity;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.util.Log;

import com.easyndk.classes.AndroidNDKHelper;
import com.pi.PantyHero.R;
import com.unicom.dcLoader.Utils;
import com.unicom.dcLoader.Utils.UnipayPayResultListener;


public class Cocos2dxActivity extends NativeActivity
{ 
	public static Purchase m_Purchase;
	private Context m_context;
	private Activity activity;
//	public IAPListener m_Listener;	
//	// 计费信息
//	// 计费信息 (现网环境)
	private static final String APPID = "300008867558";
	private static final String APPKEY = "369714FCD8C195BAC2A90AEDB975649C";
	public boolean m_bInit = false;
	// 计费点信息
	public String m_Paycode = "30000886755814";
	private ProgressDialog mProgressDialog;



	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		// 在主activity中，设置这个activity作为c++消息的接受者   
		AndroidNDKHelper.SetNDKReciever(this);
		
//		mProgressDialog = new ProgressDialog(this);
//		mProgressDialog.setIndeterminate(true);
//		mProgressDialog.setMessage("请稍候...");
		m_context = this;
		activity = this;
		
//		IAPHandler iapHandler = new IAPHandler(this);
//
//		 // IAP组件初始化.包括下面3步。
//		 // step1.实例化PurchaseListener。实例化传入的参数与您实现PurchaseListener接口的对象有关。
//		 // 例如，此Demo代码中使用IAPListener继承PurchaseListener，其构造函数需要Context实例。
//		m_Listener = new IAPListener(this, iapHandler);
//		// step2.获取Purchase实例。
//		m_Purchase = Purchase.getInstance();
//		try
//		{
//			// step3.向Purhase传入应用信息。APPID，APPKEY。 需要传入参数APPID，APPKEY。
//			m_Purchase.setAppInfo(APPID, APPKEY);
//			// step4. IAP组件初始化开始， 参数PurchaseListener，初始化函数需传入step1时实例化的onPurchaseListener
//			m_Purchase.init(m_context, m_Listener);
//			m_Purchase.setTimeout(5000, 5000);
//		}
//		catch (Exception e)
//		{
//			e.printStackTrace();
//			return;
//		}
		
		showProgressDialog();
		Utils.getInstances().initPayContext(activity, new UnipayPayResultListener(){

			@Override
			public void PayResult(String paycode, int flag, int flag2, String error) {
				// TODO Auto-generated method stub
				Log.v("xiaowo","[initPay]paycode:"+paycode+" flag:"+flag+" flag2:"+flag2+" error:"+error);
				m_bInit = true;
				dismissProgressDialog();
			}
			
		});
	}


	 /*
     * 显示对话框
     */
    public void showDialog(final String title, final String message)
    {
    	new AlertDialog.Builder(this)
    	.setTitle(title)
		.setIcon(m_context.getResources().getDrawable(R.drawable.icon))
    	.setMessage(message)
    	.setPositiveButton("确定", new DialogInterface.OnClickListener()
    	{
    		@Override
    		public void onClick(DialogInterface dialog, int which)
    		{
				//dialog.dismiss();
   			}
   		}).create().show();
    }

	private void showProgressDialog()
	{
		if (mProgressDialog == null)
		{
			mProgressDialog = new ProgressDialog(Cocos2dxActivity.this);
			mProgressDialog.setIndeterminate(true);
			mProgressDialog.setMessage("请稍候.....");
		}
		if (!mProgressDialog.isShowing())
		{
			mProgressDialog.show();
		}
	}

	public void dismissProgressDialog()
	{
		if (mProgressDialog != null && mProgressDialog.isShowing())
		{
			mProgressDialog.dismiss();
		}
	}

	@Override
	protected void onDestroy()
	{
		super.onDestroy();
	}

	
	//实现这个被c++发送消息调用的方法   
	//确保这个方法的名字跟c++传递过来的字符串相同   
	// 比如我们从c++中传递"SelectorPayRequest"字符串，这就是为什么我们创建这个方法的原因了   
	public void SelectorPayRequest(JSONObject prms)   
	{
		if (m_bInit == false)
		{
			//showDialog("天天跑吧", "初始化失败，不能购买");
		    AndroidNDKHelper.SendMessageWithParameters("SelectorPayError", null);
			return;
		}
		
//	    try  
//	    {  
//	    	m_Paycode = prms.getString("PayCode");  
//	    }  
//	    catch (JSONException e)  
//	    {  
//	        e.printStackTrace();  
//	    }  
//    	//Log.v("PantyHero", "APPID=" + APPID + ",APPKEY=" + APPKEY + ",strPayCode=" + m_Paycode);  
//	    
//		try
//		{
//			//m_Purchase.order(m_context, m_Paycode, m_Listener);
//			m_Purchase.order(m_context, m_Paycode , 1, "PantyHero", false, m_Listener);
//		}
//		catch (Exception e)
//		{
//			e.printStackTrace();
//		}
		
		Log.v("xiaowo","[SelectorPayRequest]json:"+prms.toString());
		String payCode = "013";
		try {
			payCode = prms.getString("PayCode");
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Log.v("xiaowo","[SelectorPayRequest]payCode:"+payCode);
		Utils.getInstances().pay(m_context,  payCode ,new UnipayPayResultListener(){

			@Override
			public void PayResult(String paycode, int flag, int flag2, String error) {
				Log.v("xiaowo","[pay]paycode:"+paycode+" flag:"+flag+" flag2:"+flag2+" error:"+error);
				// flag为支付回调结果，flag2为回调类型，error为当前结果描述，paycode是完整的计费点编码
				switch (flag) {
				case 1://success
					//此处放置支付请求已提交的相关处理代码
					AndroidNDKHelper.SendMessageWithParameters("SelectorPaySuccess", null);
					break;
				case 2://fail
					//此处放置支付请求失败的相关处理代码
					AndroidNDKHelper.SendMessageWithParameters("SelectorPayError", null);
					break;
				case 3://cancel
					//此处放置支付请求被取消的相关处理代码
					AndroidNDKHelper.SendMessageWithParameters("SelectorPayError", null);
					break;
				default:
					break;
				}	
			}
		});

	}

}
