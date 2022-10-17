package controller;

import view.ApplicationFrame;

public class ApplicationController implements Runnable {
    @Override
    public void run() {
        ApplicationFrame frame = new ApplicationFrame();
        frame.start();
    }
}
