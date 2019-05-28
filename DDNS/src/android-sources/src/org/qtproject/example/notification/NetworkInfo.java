package org.qtproject.example.notification;

import android.content.Context;

import android.telephony.CellInfo;
import android.telephony.CellInfoCdma;
import android.telephony.CellInfoGsm;
import android.telephony.CellInfoLte;
import android.telephony.CellIdentityLte;
import android.telephony.CellIdentityGsm;
import android.telephony.CellIdentityCdma;
import android.telephony.CellSignalStrengthCdma;
import android.telephony.CellSignalStrengthGsm;
import android.telephony.CellSignalStrengthLte;
import android.telephony.TelephonyManager;
import android.os.PowerManager;
import android.os.Bundle;
import org.json.JSONArray;
import org.json.JSONObject;
import org.json.JSONException;
import android.view.WindowManager;


public class NetworkInfo extends org.qtproject.qt5.android.bindings.QtActivity
{
    protected PowerManager.WakeLock mWakeLock;

    @Override
    public void onCreate(final Bundle icicle) {
        super.onCreate(icicle);
        final PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        //this.mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "My Tag");
        //this.mWakeLock.acquire();

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

    }

    @Override
    public void onDestroy() {
        //this.mWakeLock.release();
        super.onDestroy();
    }

    private static NetworkInfo m_instance;

    public NetworkInfo()
    {
        m_instance = this;
    }

    public static String readCellularInfo()
    {
        JSONObject data = new JSONObject();
        JSONArray cells = new JSONArray();
        try {
            final TelephonyManager tm = (TelephonyManager) m_instance.getSystemService(Context.TELEPHONY_SERVICE);

            for (final CellInfo info : tm.getAllCellInfo())
            {

                if (info instanceof CellInfoLte)
                {
                    final CellInfoLte cellInfo = (CellInfoLte)info;
                    final CellIdentityLte cellIdentity = cellInfo.getCellIdentity();
                    final CellSignalStrengthLte cellSignalStrength = cellInfo.getCellSignalStrength();

                    JSONObject entry = new JSONObject();
                    entry.put("type", "lte");
                    entry.put("isRegistered", cellInfo.isRegistered() ? 1 : 0);
                    entry.put("raw", cellSignalStrength.toString());
                    entry.put("ci", cellIdentity.getCi());
                    entry.put("mcc", cellIdentity.getMcc());
                    entry.put("mnc", cellIdentity.getMnc());
                    entry.put("pci", cellIdentity.getPci());
                    entry.put("tac", cellIdentity.getTac());

                    // parse the raw entry
                    String[] css = cellSignalStrength.toString().split(" ");
                    String ss = css[1]; ss = ss.replace("ss=", "");
                    String rsrp = css[2]; rsrp = rsrp.replace("rsrp=", "");
                    String rsrq = css[3]; rsrq = rsrq.replace("rsrq=", "");
                    String sinr = css[4]; sinr = sinr.replace("rssnr=", "");
                    String cqi = css[5]; cqi = cqi.replace("cqi=", "");
                    String ta = css[6]; ta = ta.replace("ta=", "");
                    entry.put("rsrp", Integer.parseInt(rsrp)*-1);
                    entry.put("rsrq", Integer.parseInt(rsrq));
                    entry.put("rssnr", Integer.parseInt(sinr));
                    entry.put("cqi", Integer.parseInt(cqi));
                    entry.put("ss", Integer.parseInt(ss));
                    entry.put("ta", Integer.parseInt(ta));

                    if(cellInfo.isRegistered())
                        data.put("connected", entry);
                    else
                        cells.put(entry);

                } else {
                    System.out.println("no cell info");
                }


            }
            data.put("cells", cells);
        } catch (Exception e) {
            System.out.println("exception " + e);
        }



        return data.toString();
    }
}
