using Scriptables;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UISelectorPanel : UIPanel
{
    public ElementsConfigurator configurator;


    private void Awake()
    {
        _isShowing = true;
        // Al iniciar, ocultamos el panel
        HidePanel();
    }

    public override void ShowPanel()
    {
        if (!_isShowing)
        {
            _isShowing = true;
            transform.localScale = new Vector3(1.0f, 1.0f, 1.0f);
            configurator.CheckAllLicenses();
        }
    }

    public override void HidePanel()
    {
        if (_isShowing)
        {
            _isShowing = false;
            transform.localScale = new Vector3(0.0f, 0.0f, 1.0f);
        }
    }
}
