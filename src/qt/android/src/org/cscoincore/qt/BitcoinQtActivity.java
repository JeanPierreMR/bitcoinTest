package org.cscoincore.qt;

import android.os.Bundle;
import android.system.ErrnoException;
import android.system.Os;

import org.qtproject.qt5.android.bindings.QtActivity;

import java.io.File;

public class CScoinQtActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        final File cscoinDir = new File(getFilesDir().getAbsolutePath() + "/.cscoin");
        if (!cscoinDir.exists()) {
            cscoinDir.mkdir();
        }

        super.onCreate(savedInstanceState);
    }
}
