package com.baidu.paddle.lite.demo.ppocr_demo;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.support.annotation.NonNull;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.TextView;

import com.baidu.paddle.lite.demo.common.Utils;
import com.example.ocrtext.BocOcrPredictor;
import com.example.ocrtext.OcrResultBean;

import java.io.File;
import java.io.InputStream;
import java.util.List;


public class MainActivity2 extends Activity {
    private static final String TAG = MainActivity2.class.getSimpleName();

    // Model settings of object detection
    protected String detModelPath = "ch_ppocr_mobile_v2.0_det_slim_opt.nb";
    protected String recModelPath = "ch_ppocr_mobile_v2.0_rec_slim_opt.nb";
    protected String clsModelPath = "ch_ppocr_mobile_v2.0_cls_slim_opt.nb";
    protected String labelPath = "ppocr_keys_v1.txt";
    protected String configPath = "config.txt";
    protected int cpuThreadNum = 1;
    protected String cpuPowerMode = "LITE_POWER_HIGH";
    private TextView  textView;


    // Native predictor = new Native();
    BocOcrPredictor predictor = new BocOcrPredictor();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // Fullscreen
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_main2);
        setupView();

    }

    private void setupView() {
        Button initButton = findViewById(R.id.init_button);
        Button selectButton = findViewById(R.id.select_button);
        initButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                checkRun();
            }
        });
        selectButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                openGallery();
            }
        });
        textView=findViewById(R.id.text);

    }

    private static final int GALLERY_REQUEST_CODE = 200;

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == GALLERY_REQUEST_CODE && resultCode == RESULT_OK && data != null) {
            Uri selectedImageUri = data.getData();
            if (selectedImageUri != null) {
                // 获取图片路径或直接使用 Uri
                Bitmap image = getBitmapFromUri(selectedImageUri);
                // 显示图片或进行其他操作
                List<OcrResultBean> results = predictor.recognise(image);
                   if(results==null){
                       return;
                   }
                for (int i = 0; i < results.size(); i++) {
                    final String res = results.get(i).text+" "+results.get(i).score;
                    Log.i("yll", "index=" + i + "   " + res);
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            StringBuffer  stringBuffer=new StringBuffer(textView.getText().toString());
                            stringBuffer.append("\n");
                            stringBuffer.append(res);
                            textView.setText(stringBuffer.toString());
                        }
                    });
                }
            }
        }
    }

    private Bitmap getBitmapFromUri(Uri uri) {
        try {
            InputStream inputStream = getContentResolver().openInputStream(uri);
            if (inputStream != null) {
                // 设置采样率
                BitmapFactory.Options options = new BitmapFactory.Options();
                options.inPreferredConfig = Bitmap.Config.ARGB_8888;
                Bitmap bitmap = BitmapFactory.decodeStream(inputStream, null, options);
                inputStream.close();
                return bitmap;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    private void openGallery() {
        Intent intent = new Intent(Intent.ACTION_PICK, MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
        startActivityForResult(intent, 200);
    }


    public void checkRun() {
        try {
            Utils.copyAssets(this, labelPath);
            String labelRealDir = new File(this.getExternalFilesDir(null), labelPath).getAbsolutePath();

            Utils.copyAssets(this, configPath);
            String configRealDir = new File(this.getExternalFilesDir(null), configPath).getAbsolutePath();

            Utils.copyAssets(this, detModelPath);
            String detRealModelDir = new File(this.getExternalFilesDir(null), detModelPath).getAbsolutePath();

            Utils.copyAssets(this, clsModelPath);
            String clsRealModelDir = new File(this.getExternalFilesDir(null), clsModelPath).getAbsolutePath();

            Utils.copyAssets(this, recModelPath);
            String recRealModelDir = new File(this.getExternalFilesDir(null), recModelPath).getAbsolutePath();

            predictor.init(this, detRealModelDir, clsRealModelDir, recRealModelDir, configRealDir, labelRealDir, cpuThreadNum, cpuPowerMode);
        } catch (Throwable e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (grantResults[0] != PackageManager.PERMISSION_GRANTED || grantResults[1] != PackageManager.PERMISSION_GRANTED) {
            new AlertDialog.Builder(MainActivity2.this).setTitle("Permission denied").setMessage("Click to force quit the app, then open Settings->Apps & notifications->Target " + "App->Permissions to grant all of the permissions.").setCancelable(false).setPositiveButton("Exit", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    MainActivity2.this.finish();
                }
            }).show();
        }
    }


}
