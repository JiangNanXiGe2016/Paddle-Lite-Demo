package com.example.ocrtext;

public class OcrResultBean {
    public  String   text;
    public  float    score;

    public OcrResultBean(String text, float score) {
        this.text = text;
        this.score = score;
    }
}
