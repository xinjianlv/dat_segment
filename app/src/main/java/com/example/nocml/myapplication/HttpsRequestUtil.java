/*=============================================================================
#     FileName: Action.java
#         Desc: http请求服务工具类
#       Author: Bruce LV
#        Email: xinjian.lv@gmail.com
#     HomePage: null
#      Version: 0.0.1
#   LastChange: 2017-07-18 09:04:40
#      History:
=============================================================================*/
package com.example.nocml.myapplication;


import javax.net.ssl.HostnameVerifier;
import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLSession;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URL;
import java.net.URLConnection;


public class HttpsRequestUtil {

	private static String respEncode = "utf-8";//"UTF-8";

	public static void main(String[] args) {
		String term = getDataByHttp("http://localhost:11765/?q=今天好冷");
		System.out.println("term:" + term);
	}
	public static String getDataByHttps(String url) {
		try {
			URL myURL = new URL(url);

			// 创建HttpsURLConnection对象，并设置其SSLSocketFactory对象
			trustAllHttpsCertificates();
			HttpsURLConnection.setDefaultHostnameVerifier(hv);
			HttpsURLConnection httpsConn = (HttpsURLConnection) myURL
					.openConnection();

			// 取得该连接的输入流，以读取响应内容
			InputStreamReader insr = new InputStreamReader(
					httpsConn.getInputStream(), getRespEncode());

			// 读取服务器的响应内容并显示
			int respInt = insr.read();
			StringBuffer sb = new StringBuffer();
			while (respInt != -1) {
				sb.append((char) respInt);
				respInt = insr.read();
			}
			return sb.toString();
		} catch (Exception e) {
			return null;
		}
	}

	public static String getDataByHttp(String urlStr) {

		String result = "";
		BufferedReader in = null;
		try {

			// URL realUrl = new URL(urlName);
			URL realUrl = new URL(urlStr);
			// 打开和URL之间的连接
			URLConnection conn = realUrl.openConnection();
			// conn.setFileNameMap(filenamemap);
			// 设置通用的请求属性
			conn.setRequestProperty("accept", "*/*");
			conn.setRequestProperty("connection", "Keep-Alive");
			conn.setRequestProperty("user-agent",
					"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)");
			// 建立实际的连接
			conn.connect();
			// 获取所有响应头字段
			// 遍历所有的响应头字段
			// for (String key : map.keySet()) {
			// System.out.println(key + "---&gt;" + map.get(key));
			// }

			// 定义BufferedReader输入流来读取URL的响应
			in = new BufferedReader(new InputStreamReader(
					conn.getInputStream(), getRespEncode()));
			String line;
			while ((line = in.readLine()) != null) {
				if(result.length() > 0)
					result += "\n";
				result +=  line;
			}
		} catch (Exception e) {
			e.printStackTrace();

		}
		// 使用finally块来关闭输入流
		finally {
			try {
				if (in != null) {
					in.close();
				}
			} catch (IOException ex) {
				ex.printStackTrace();
			}
		}
		return result;
	}


	public static String getRespEncode() {
		return respEncode;
	}

	public static HostnameVerifier hv = new HostnameVerifier() {
		public boolean verify(String urlHostName, SSLSession session) {
			return true;
		}
	};

	private static void trustAllHttpsCertificates() throws Exception {
		javax.net.ssl.TrustManager[] trustAllCerts = new javax.net.ssl.TrustManager[1];
		javax.net.ssl.TrustManager tm = new miTM();
		trustAllCerts[0] = tm;
		javax.net.ssl.SSLContext sc = javax.net.ssl.SSLContext
				.getInstance("SSL");
		sc.init(null, trustAllCerts, null);
		javax.net.ssl.HttpsURLConnection.setDefaultSSLSocketFactory(sc
				.getSocketFactory());
	}

	static class miTM implements javax.net.ssl.TrustManager,
			javax.net.ssl.X509TrustManager {
		public java.security.cert.X509Certificate[] getAcceptedIssuers() {
			return null;
		}

		public boolean isServerTrusted(
				java.security.cert.X509Certificate[] certs) {
			return true;
		}

		public boolean isClientTrusted(
				java.security.cert.X509Certificate[] certs) {
			return true;
		}

		public void checkServerTrusted(
				java.security.cert.X509Certificate[] certs, String authType)
				throws java.security.cert.CertificateException {
			return;
		}

		public void checkClientTrusted(
				java.security.cert.X509Certificate[] certs, String authType)
				throws java.security.cert.CertificateException {
			return;
		}
	}

}
