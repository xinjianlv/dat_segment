package com.example.nocml.myapplication;

import android.graphics.Color;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

import com.alibaba.fastjson.JSONObject;
import com.google.android.gms.appindexing.Action;
import com.google.android.gms.appindexing.AppIndex;
import com.google.android.gms.appindexing.Thing;
import com.google.android.gms.common.api.GoogleApiClient;


public class MainActivity extends AppCompatActivity {
    private final String server = "http://127.0.0.1:11765/?q=";
    private TextView Text = null;
    private int ANDROID_ACCESS_CXF_WEBSERVICES = 001;
    private Handler handler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            String result = (String) msg.getData().get("result");
            String obj = (String) msg.obj;//
            Text.setText("请求结果为："+result);
        }

    };

    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    private GoogleApiClient client;

//    @SuppressLint("NewApi")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        Text = (TextView) findViewById(R.id.text1);
        final Button Button1 = (Button) findViewById(R.id.button1);
        final Button Button2 = (Button) findViewById(R.id.button2);

        Button1.setOnClickListener(new OnClickListener() { // 实现行为功能
            public void onClick(View v) {
                Text.setBackgroundColor(Color.RED);
                Text.append("T");
            }
        });
        Button2.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                Thread accessWebServiceThread = new Thread(new WebServiceHandler());
                accessWebServiceThread.start();
            }
        });

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client = new GoogleApiClient.Builder(this).addApi(AppIndex.API).build();
    }

    public String getAnswer(String question) {
        try {
            String result = "";
            String qString = "http://api.ffan.com/movie/v3/cinemas/13856/plays";//server + question;
//            String qString = server + question;
            String jsonRe = HttpsRequestUtil.getDataByHttp(qString);
            if (jsonRe != null && jsonRe.length() > 0) {
                JSONObject j = JSONObject.parseObject(jsonRe);
                result = j.getString("tip");
            }

            return result;
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }
    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    public Action getIndexApiAction() {
        Thing object = new Thing.Builder()
                .setName("Main Page") // TODO: Define a title for the content shown.
                // TODO: Make sure this auto-generated URL is correct.
                .setUrl(Uri.parse("http://[ENTER-YOUR-URL-HERE]"))
                .build();
        return new Action.Builder(Action.TYPE_VIEW)
                .setObject(object)
                .setActionStatus(Action.STATUS_TYPE_COMPLETED)
                .build();
    }
    class WebServiceHandler implements Runnable{
        @Override
        public void run() {
            Looper.prepare();
            String result = getAnswer("你好！");
            Message message = new Message();
            Bundle bundle = new Bundle();
            bundle.putString("result", result);
            message.what = ANDROID_ACCESS_CXF_WEBSERVICES;//设置消息标示
            message.obj = "zxn";
            message. setData(bundle);//消息内容
            handler.sendMessage(message);//发送消息
            Looper.loop();
        }

    }
    @Override
    public void onStart() {
        super.onStart();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client.connect();
        AppIndex.AppIndexApi.start(client, getIndexApiAction());
    }

    @Override
    public void onStop() {
        super.onStop();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        AppIndex.AppIndexApi.end(client, getIndexApiAction());
        client.disconnect();
    }
}
