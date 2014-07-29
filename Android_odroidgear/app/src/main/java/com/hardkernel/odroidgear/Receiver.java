package com.hardkernel.odroidgear;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.telephony.PhoneStateListener;
import android.telephony.SmsMessage;
import android.telephony.TelephonyManager;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

/**
 * Created by john on 14. 7. 9.
 */

public class Receiver extends BroadcastReceiver {

    private Socket socket;
    private Context mContext;
    private Intent mIntent;
    private int cnt = 0;
    private static String str = "";
    private static final int SERVERPORT = 8080;
    private static final String SERVER_IP = "192.168.0.2";

    static final String logTag = "SmsReceiver";
    static final String smsACTION = "android.provider.Telephony.SMS_RECEIVED";
    static final String callACTION = "android.intent.action.PHONE_STATE";

    @Override
    public void onReceive(final Context context, Intent intent) {
        mContext = context;
        mIntent = intent;
        new Thread(new ClientThread()).start();
        TelephonyManager tm = (TelephonyManager) context.getSystemService(context.TELEPHONY_SERVICE);
        int events = PhoneStateListener.LISTEN_CALL_STATE;
        tm.listen(phoneStateListener, events);

        if (intent.getAction().equals(smsACTION)) {
            Bundle bundle = intent.getExtras();
            if (bundle == null) {
                return;
            }
            Object[] pdusObj = (Object[]) bundle.get("pdus");
            if (pdusObj == null) {
                return;
            }
            SmsMessage[] smsMessages = new SmsMessage[pdusObj.length];
            for (int i = 0; i < pdusObj.length; i++) {
                smsMessages[i] = SmsMessage.createFromPdu((byte[]) pdusObj[i]);
                str = "";
                str += "n" + smsMessages[i].getOriginatingAddress();
                str += "e";
                str += "m" + smsMessages[i].getMessageBody();
            }
            try {
                while (socket == null) ;
                PrintWriter out = new PrintWriter(new BufferedWriter(
                        new OutputStreamWriter(socket.getOutputStream())),
                        true
                );
                out.printf("%s", str);
            } catch (UnknownHostException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private final PhoneStateListener phoneStateListener = new PhoneStateListener() {
        @Override

        public void onCallStateChanged(int state, String incomingNumber) {
            if (mIntent.getAction().equals(callACTION)) {
                str = "";
                str += "n" + incomingNumber;
                str += "e";
                str += "s";
                switch (state) {
                    case TelephonyManager.CALL_STATE_IDLE:
                        str += "i";
                        break;
                    case TelephonyManager.CALL_STATE_RINGING:
                        str += "r";
                        break;
                    case TelephonyManager.CALL_STATE_OFFHOOK:
                        str += "o";
                        break;
                }
                super.onCallStateChanged(state, incomingNumber);
                try {
                    while (socket == null) ;
                    PrintWriter out = new PrintWriter(new BufferedWriter(
                            new OutputStreamWriter(socket.getOutputStream())),
                            true );
                    out.printf("%s", str);
                } catch (UnknownHostException e) {
                    e.printStackTrace();
                } catch (IOException e) {
                    e.printStackTrace();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    };

    class ClientThread implements Runnable {
        @Override
        public void run() {
            try {
                InetAddress serverAddr = InetAddress.getByName(SERVER_IP);
                socket = new Socket(serverAddr, SERVERPORT);
            } catch (UnknownHostException e1) {
                e1.printStackTrace();
            } catch (IOException e1) {
                e1.printStackTrace();
            }
        }
    }
}