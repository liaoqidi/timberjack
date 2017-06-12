package org.YiDongMM;

import java.util.HashMap;

import org.cocos2dx.cpp.Cocos2dxActivity;

import com.easyndk.classes.AndroidNDKHelper;

import mm.purchasesdk.OnPurchaseListener;
import mm.purchasesdk.Purchase;
import mm.purchasesdk.core.PurchaseCode;
//import mm.purchasesdk.PurchaseCode;
import android.content.Context;
import android.os.Message;
import android.util.Log;

public class IAPListener implements OnPurchaseListener
{
	private final String TAG = "PantyHero.IAPListener";
	private Cocos2dxActivity m_context;
	private IAPHandler iapHandler;

	public IAPListener(Context context, IAPHandler iapHandler)
	{
		this.m_context = (Cocos2dxActivity) context;
		this.iapHandler = iapHandler;
		//m_context.showDialog(TAG, "new IAPListener");
	}

	@Override
	public void onAfterApply() 
	{
	}

	@Override
	public void onAfterDownload() 
	{
	}

	@Override
	public void onBeforeApply() 
	{
	}

	@Override
	public void onBeforeDownload() 
	{
	}

	//��ʼ������ӿڡ������ߵ����˳�ʼ���ӿں�������ڴ˺����б�����
	@Override
	public void onInitFinish(String  code)
	{
		//m_context.showDialog(TAG, "Init finish, status code = " + code);
		m_context.m_bInit = true;
		Log.d(TAG, "Init finish, status code = " + code);
		Message message = iapHandler.obtainMessage(IAPHandler.INIT_FINISH);
		String result = "初始化结果："+ Purchase.getReason(code);
		Log.d(TAG, result);
		message.obj = result;
		message.sendToTarget();
	}

	//��������ӿڡ�
	@Override
	public void onBillingFinish(String code, HashMap arg1) 
	{
		//m_context.showDialog(TAG, "billing finish, status code = " + code);
		Log.d(TAG, "billing finish, status code = " + code);
		String result = "订购结果：订购成功";
		Message message = iapHandler.obtainMessage(IAPHandler.BILL_FINISH);
		// �˴ζ�����orderID
		String orderID = null;
		// ��Ʒ��paycode
		String paycode = null;
		// ��Ʒ����Ч��(������������Ʒ��Ч)
		String leftday = null;
		// ��Ʒ�Ľ��� ID���û����Ը�������ID����ѯ��Ʒ�Ƿ��Ѿ�����
		String tradeID = null;
		
		String ordertype = null;
		if ( PurchaseCode.BILL_ORDER_OK.equalsIgnoreCase( code )
				|| PurchaseCode.AUTH_OK.equalsIgnoreCase( code )
				|| PurchaseCode.WEAK_ORDER_OK.equalsIgnoreCase( code ) ) {
		
			if (arg1 != null) {
				leftday = (String) arg1.get(OnPurchaseListener.LEFTDAY);
				if (leftday != null && leftday.trim().length() != 0) {
					result = result + ",剩余时间 ： " + leftday;
				}
				orderID = (String) arg1.get(OnPurchaseListener.ORDERID);
				if (orderID != null && orderID.trim().length() != 0) {
					result = result + ",OrderID ： " + orderID;
				}
				paycode = (String) arg1.get(OnPurchaseListener.PAYCODE);
				if (paycode != null && paycode.trim().length() != 0) {
					result = result + ",Paycode:" + paycode;
				}
				tradeID = (String) arg1.get(OnPurchaseListener.TRADEID);
				if (tradeID != null && tradeID.trim().length() != 0) {
					result = result + ",tradeID:" + tradeID;
				}
				ordertype = (String) arg1.get(OnPurchaseListener.ORDERTYPE);
				if (tradeID != null && tradeID.trim().length() != 0) {
					result = result + ",ORDERTYPE:" + ordertype;
				}
			}
		    AndroidNDKHelper.SendMessageWithParameters("SelectorPaySuccess", null);
		} else {
			/**
			 * 表示订购失败。
			 */
			result = "订购结果：" + Purchase.getReason(code);
			  AndroidNDKHelper.SendMessageWithParameters("SelectorPayError", null);
		}		
			
			
//			if (arg1 != null) 
//			{
//				leftday = (String) arg1.get(OnPurchaseListener.LEFTDAY);
//				if (leftday != null && leftday.trim().length() != 0) 
//				{
//					result = result + ",ʣ��ʱ�� �� " + leftday;
//				}
//				orderID = (String) arg1.get(OnPurchaseListener.ORDERID);
//				if (orderID != null && orderID.trim().length() != 0) 
//				{
//					result = result + ",OrderID �� " + orderID;
//				}
//				paycode = (String) arg1.get(OnPurchaseListener.PAYCODE);
//				if (paycode != null && paycode.trim().length() != 0) 
//				{
//					result = result + ",Paycode:" + paycode;
//				}
//				tradeID = (String) arg1.get(OnPurchaseListener.TRADEID);
//				if (tradeID != null && tradeID.trim().length() != 0) 
//				{
//					result = result + ",tradeID:" + tradeID;
//				}
//				ordertype = (String) arg1.get(OnPurchaseListener.ORDERTYPE);
//				if (tradeID != null && tradeID.trim().length() != 0) 
//				{
//					result = result + ",ORDERTYPE:" + ordertype;
//				}
//			}
//			  
//		    // ���ʹ�������Ϣ�����SelectorPaySuccess�б����������ͬ���ֵĺ���c++�Ż������Щ��Ϣ��
//		    AndroidNDKHelper.SendMessageWithParameters("SelectorPaySuccess", null);
//		}
//		else 
//		{
//			/**
//			 * ��ʾ����ʧ�ܡ�
//			 */
//			result = "�������" + Purchase.getReason(code);
//			
//		    // ���ʹ�������Ϣ�����SelectorPayError�б����������ͬ���ֵĺ���c++�Ż������Щ��Ϣ��
//		    AndroidNDKHelper.SendMessageWithParameters("SelectorPayError", null);
//		}
		m_context.dismissProgressDialog();
		Log.d(TAG, result);
		System.out.println(result);
		
		message.obj = result;
		message.sendToTarget();
	}

	//��ѯ����ӿ�
	@Override
	public void onQueryFinish(String code, HashMap<String, Object> arg1) 
	{
		//m_context.showDialog(TAG, "license finish, status code = " + code);
		Log.d(TAG, "license finish, status code = " + code);
		Message message = iapHandler.obtainMessage(IAPHandler.QUERY_FINISH);
		String result = "查询成功,该商品已购买";
		// �˴ζ�����orderID
		String orderID = null;
		// ��Ʒ��paycode
		String paycode = null;
		// ��Ʒ����Ч��(������������Ʒ��Ч)
		String leftday = null;
		if (code != PurchaseCode.QUERY_OK) 
		{
			/**
			 * ��ѯ������Ʒ����������Ϣ
			 */
			result = "查询结果：" + Purchase.getReason(code);
		} 
		else 
		{
			/**
			 * ��ѯ����Ʒ�������Ϣ��
			 * ��ʱ����Ի����Ʒ��paycode��orderid���Լ���Ʒ����Ч��leftday��������������Ʒ���Է��أ�
			 */
			leftday = (String) arg1.get(OnPurchaseListener.LEFTDAY);
			if (leftday != null && leftday.trim().length() != 0) 
			{
				result = result + ",剩余时间 ： "  + leftday;
			}
			orderID = (String) arg1.get(OnPurchaseListener.ORDERID);
			if (orderID != null && orderID.trim().length() != 0) 
			{
				result = result + ",OrderID: " + orderID;
			}
			paycode = (String) arg1.get(OnPurchaseListener.PAYCODE);
			if (paycode != null && paycode.trim().length() != 0) 
			{
				result = result + ",Paycode:" + paycode;
			}
		}
		System.out.println(result);
		m_context.dismissProgressDialog();
		message.obj = result;
		message.sendToTarget();
	}

	
	//�˶��ӿڣ�
	@Override
	public void onUnsubscribeFinish(String  code) 
	{
		String result = "退订结果：" + Purchase.getReason(code);
		//m_context.showDialog(TAG, result);
		System.out.println(result);
		m_context.dismissProgressDialog();
		
		Message message = iapHandler.obtainMessage(IAPHandler.UNSUB_FINISH);
		message.obj = result;
		message.sendToTarget();
	}







}
