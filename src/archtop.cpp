#include <wx/wx.h>
#include <memory>
#include <point.h>
#include <arc.h>
#include <ray.h>
#include <linesegment.h>
#include <drawing.h>
#include <parsedxf.h>
#include <dxflib/dl_dxf.h>

class Archtop: public wxApp {
    public:
        virtual bool OnInit();
};

class MainFrame: public wxFrame {
    public:
        MainFrame(wxString const& title, wxPoint const& pos, wxSize const& size);

    private:
        void OnExit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnRun(wxCommandEvent& event);
        wxDECLARE_EVENT_TABLE();

        wxWindow* m_drawingWindow;
};

enum {
    ID_RUN = 1
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(Archtop);


bool Archtop::OnInit() {
    MainFrame * frame(new MainFrame("Archtop", wxPoint(200,200), wxSize(200, 200)));
    frame->Show(true);
    return true;
}

void MainFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event) {
}

void MainFrame::OnRun(wxCommandEvent& event) {
    //calculate the max
    wxClientDC dc(m_drawingWindow);

    dc.SetBrush(wxNullBrush); //turn off fill
    wxPen myPen(*wxBLACK, 2);
    dc.SetPen(myPen);

    //dc.SetMapMode(wxMM_METRIC);
    dc.SetAxisOrientation(true, true);


    DL_Dxf dxf;
    ParseDXF parser;
    dxf.in("test.dxf", &parser);

    wxSize dcSize(dc.GetSize()); //size of the 

    double minX = parser.GetDrawing().MinX();
    double minY = parser.GetDrawing().MinY();
    double maxX = parser.GetDrawing().MaxX();
    double maxY = parser.GetDrawing().MaxY();

    double xSpan = maxX;
    double ySpan = maxY;
    double drawingAspect = ySpan/xSpan;
    double screenAspect=(dcSize.y*1.0)/dcSize.x;
    double scale=dcSize.x/xSpan;

    if (scale*ySpan > dcSize.y) {
        scale = dcSize.y/ySpan;
    }

    dc.SetDeviceOrigin(0, dcSize.y);

    parser.GetDrawing().Draw(dc, scale);

    LineSegment xOrig(pt(0,0), pt(100,0));
    LineSegment yOrig(pt(0,0), pt(0,100));

    //mark drawing center and create X/Y axes

    pt_base<double> center = parser.GetDrawing().GetCenter();
    dc.CrossHair(center.x*scale, center.y*scale);
    xOrig.Draw(dc, scale);
    yOrig.Draw(dc, scale);


    for (double pct=0.80; pct > 0.2; pct-=0.10) {

        std::cout << "Drawing at " << pct*100 << "%" << std::endl;
        std::shared_ptr<Drawing> reduce1(parser.GetDrawing().Reduce(pct, dc, scale));
        reduce1->Draw(dc,scale);
    }


    /*std::shared_ptr<Drawing> reduce1(parser.GetDrawing().Reduce(0.8, dc, scale));
    reduce1->Draw(dc,scale);
    std::shared_ptr<Drawing> reduce2(reduce1->Reduce(0.75, dc, scale));
    reduce2->Draw(dc,scale);
    std::shared_ptr<Drawing> reduce3(reduce2->Reduce(0.67, dc, scale));
    reduce3->Draw(dc,scale);
    std::shared_ptr<Drawing> reduce4(reduce3->Reduce(0.5, dc, scale));
    reduce4->Draw(dc,scale);*/
    /*std::shared_ptr<Drawing> reduce3(parser.GetDrawing().Reduce(0.7, dc, scale));
    reduce3->Draw(dc,scale);
    std::shared_ptr<Drawing> reduce4(parser.GetDrawing().Reduce(0.60, dc, scale));
    reduce4->Draw(dc,scale);
    std::shared_ptr<Drawing> reduce5(parser.GetDrawing().Reduce(0.50, dc, scale));
    reduce5->Draw(dc,scale);
    std::shared_ptr<Drawing> reduce6(parser.GetDrawing().Reduce(0.40, dc, scale));
    reduce6->Draw(dc,scale);
    std::shared_ptr<Drawing> reduce7(parser.GetDrawing().Reduce(0.30, dc, scale));
    reduce7->Draw(dc,scale);*/
}

MainFrame::MainFrame(wxString const& title, wxPoint const& pos, wxSize const& size) 
    : wxFrame(NULL, wxID_ANY, title, pos, size)
    , m_drawingWindow(NULL) {
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);


    //create drawing area
    m_drawingWindow = new wxWindow(this, 
                                   wxID_ANY, 
                                   wxDefaultPosition,
                                   wxSize(400,400),
                                   (wxBORDER_SIMPLE));
    m_drawingWindow->SetMinSize(wxSize(400,400));

    //create control buttons
    wxButton* runButton = new wxButton(this, ID_RUN, "Run", wxDefaultPosition, wxSize(-1, 26) );
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(runButton,
                      wxSizerFlags()
                       .FixedMinSize()
                       .Left());
    buttonSizer->AddStretchSpacer();


    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(m_drawingWindow, 
                   wxSizerFlags().Expand()
                                 .Top()
                                 .Left()
                                 .Proportion(1));
    mainSizer->Add(buttonSizer, 
                   wxSizerFlags()
                    .FixedMinSize()
                    .Bottom()
                    .Left()
                    .Border(wxALL, 10)
                    .Proportion(0));


    //add the main sizer to our frame
    SetSizerAndFit(mainSizer);


    CreateStatusBar();
    SetStatusText("Archtop");

    Connect(ID_RUN, wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(MainFrame::OnRun));
}

