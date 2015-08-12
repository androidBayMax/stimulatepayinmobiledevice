package com.tencent.stimulatepayinmoblie;

import android.app.Activity;
import android.app.AlertDialog;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.llg.stimulatepayinmoblie.R;

public class MainActivity extends Activity implements OnClickListener {

	private EditText et_name;
	private EditText et_password;
	private EditText et_money;
	private Button bt;
	private AlertDialog dialog;
	static{
		System.loadLibrary("stimulatepay");
	}
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		et_name=(EditText) findViewById(R.id.et_name);
		et_password=(EditText) findViewById(R.id.et_password);
		et_money=(EditText) findViewById(R.id.et_money);
		bt=(Button) findViewById(R.id.bt);
		bt.setOnClickListener(this);
	}

	//在这里完成页面显示,在c中完成安全支付
	/**
	 * 
	 * @param username
	 * @param password
	 * @param money
	 * @return 200,支付成功;403,余额不足;404,用户名和密码错误
	 */
	 
	public native int safePay(String username,String password,float money);

	@Override
	public void onClick(View v) {
		final String username = et_name.getText().toString().trim();
		final String pw =et_password.getText().toString().trim();
		String money_s=et_money.getText().toString().trim();
		final float money=Float.parseFloat(money_s);
		if(v==bt)
		{
			//点击了支付
			//定义为耗时操作,在子线程中执行
			new Thread(){
				public void run() {
					final int num = safePay(username, pw, money);
					runOnUiThread(new Runnable() {
						
						@Override
						public void run() {
							switch (num) {
							case 200:
								Toast.makeText(getApplicationContext(), "支付成功", 1).show();
								break;
							case 403:
								Toast.makeText(getApplicationContext(), "支付失败,余额不足", 1).show();
								break;
							case 404:
								Toast.makeText(getApplicationContext(), "支付失败,用户名或密码不正确", 1).show();
								break;

							default:
								break;
							}
						}
					});
				};
			}.start();
		}
	}
	public void showDialog(final String msg){
		if(dialog!=null)
		{
			dialog.dismiss();
			dialog=null;
		}
		runOnUiThread(new Runnable() {
			
			@Override
			public void run() {
				AlertDialog.Builder builder=new AlertDialog.Builder(MainActivity.this);
				builder.setTitle("提示");
				builder.setMessage(msg);
				dialog = builder.show();
			}
		});
	}
	public void dismissDialog(){
		dialog.dismiss();
	}

}
