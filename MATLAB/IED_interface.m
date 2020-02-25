% /******************************************************************************
% 
%    Project:	     Alex
% 
%    Filename:       IED_interface.m
% 
%    Description:    MATLAB graphical user interface for IED (needs corresponding
%                    IED_interface.fig)
% 
%    © Copyright 2019 KTH. All rights reserved.
% 
% /******************************************************************************

function varargout = IED_interface(varargin)

% IMPORTANT
% The channel data is starting from ch0 and counting upwards and fits to the IED numbering of channels. 
% The actual plot starts with "channel 1" and fits RTDS numbering.
% 

% colors
% ch0,8  - blue - b
% ch1,9  - red - r
% ch2,10 - cyan - c
% ch3,11 - magenta - m
% ch4,12 - steel blue [0.2734 0.5078 0.7031]
% ch5,13 - black - k
% ch6,14 - orange - [1 0.6 0]
% ch7,15 - darkgreen - [0 0.5 0]
% 8   - silver [0.75 0.75 0.75]
% 9   - darkcyan [0 0.5430 0.5430]
% trip - green - g
% ch0-7 : positive pole, solid line, scatter 1/10
% ch8-16: negative pole, dashedline, scatter 1/10

%IED_INTERFACE MATLAB code file for IED_interface.fig
%      IED_INTERFACE, by itself, creates a new IED_INTERFACE or raises the existing
%      singleton*.
%
%      H = IED_INTERFACE returns the handle to a new IED_INTERFACE or the handle to
%      the existing singleton*.
%
%      IED_INTERFACE('Property','Value',...) creates a new IED_INTERFACE using the
%      given property value pairs. Unrecognized properties are passed via
%      varargin to IED_interface_OpeningFcn.  This calling syntax produces a
%      warning when there is an existing singleton*.
%
%      IED_INTERFACE('CALLBACK') and IED_INTERFACE('CALLBACK',hObject,...) call the
%      local function named CALLBACK in IED_INTERFACE.M with the given input
%      arguments.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help IED_interface

% Last Modified by GUIDE v2.5 14-May-2019 18:23:54

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @IED_interface_OpeningFcn, ...
                   'gui_OutputFcn',  @IED_interface_OutputFcn, ...
                   'gui_LayoutFcn',  [], ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
   gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before IED_interface is made visible.
function IED_interface_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   unrecognized PropertyName/PropertyValue pairs from the
%            command line (see VARARGIN)

% Choose default command line output for IED_interface
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);
% UIWAIT makes IED_interface wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = IED_interface_OutputFcn(hObject, eventdata, handles)
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;



function COM = edit_COM_Callback(handles)
COM = ['COM' handles.edit_COM.String];


% --- Executes during object creation, after setting all properties.
function edit_COM_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

% --- Executes when selected object is changed in unitgroup.
function unitgroup_SelectionChangedFcn(hObject, eventdata, handles)

% --- Executes on button press in V.
function V_Callback(hObject, eventdata, handles)
axes(handles.axes2);
yyaxis left;
Ytick = get(handles.axes2, 'Ytick');
yticklabels(cellstr(num2str(Ytick')));
ylabel('voltage [V]');
yyaxis right;
Ytick = get(handles.axes2, 'Ytick');
yticklabels(cellstr(num2str(Ytick')));
ylabel('voltage [V]');

% --- Executes on button press in kV.
function kV_Callback(hObject, eventdata, handles)
scale_factor_voltage = edit_scale_factor_voltage_Callback(handles);
scale_factor_current = edit_scale_factor_current_Callback(handles);
axes(handles.axes2);
yyaxis left;
Ytick = get(handles.axes2, 'Ytick');
Ytick_scale = Ytick*scale_factor_voltage;
yticklabels(cellstr(num2str(Ytick_scale')));
ylabel('voltage [kV]');
yyaxis right;
Ytick = get(handles.axes2, 'Ytick');
Ytick_scale = Ytick*scale_factor_current;
yticklabels(cellstr(num2str(Ytick_scale')));
ylabel('current [kA]');

function scale_factor_voltage = edit_scale_factor_voltage_Callback(handles)
scale_factor_voltage = str2double(get(handles.edit_scale_factor_voltage,'string'));

function scale_factor_current = edit_scale_factor_current_Callback(handles)
scale_factor_current = str2double(get(handles.edit_scale_factor_current,'string'));

function pu_factor_voltage = edit_pu_factor_voltage_Callback(handles)
pu_factor_voltage = str2double(get(handles.edit_pu_factor_voltage,'string'));

function pu_factor_current = edit_pu_factor_current_Callback(handles)
pu_factor_current = str2double(get(handles.edit_pu_factor_current,'string'));



% --- Executes during object creation, after setting all properties.
function edit_scale_factor_voltage_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
% --- Executes during object creation, after setting all properties.
function edit_scale_factor_current_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
% --- Executes during object creation, after setting all properties.
function edit_pu_factor_voltage_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
% --- Executes during object creation, after setting all properties.
function edit_pu_factor_current_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


function kHz25_Callback(hObject, eventdata, handles)


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% READ MEASUREMENTS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function update_measurements_Callback(hObject, eventdata, handles)
    handles.wait_text.BackgroundColor = 'r';
    handles.wait_text.String = 'WAIT';
    
    update_trip_Callback(handles);
    ch_length = 800;
    ch_data = (0:1:ch_length).*0;
    
    if (handles.show_ch0.Value) read_ch('ch0', '1p - voltage', ch_data, 'blue', ch_length, handles.axes2, 'left', 'pos', handles); end
    if (handles.show_ch1.Value) read_ch('ch1', '1p - current', ch_data, 'red', ch_length, handles.axes2, 'right', 'pos', handles); end
    if (handles.show_ch2.Value) read_ch('ch2', '2p - voltage', ch_data, 'cyan', ch_length, handles.axes2, 'left', 'pos', handles); end
    if (handles.show_ch3.Value) read_ch('ch3', '2p - current', ch_data, 'magenta', ch_length, handles.axes2, 'right', 'pos', handles); end
    if (handles.show_ch4.Value) read_ch('ch4', '3p - voltage', ch_data, '[0.2734 0.5078 0.7031]', ch_length, handles.axes2, 'left', 'pos', handles); end
    if (handles.show_ch5.Value) read_ch('ch5', '3p - current', ch_data, 'black', ch_length, handles.axes2, 'right', 'pos', handles); end
    if (handles.show_ch6.Value) read_ch('ch6', 'busp - voltage', ch_data, [1 0.6 0], ch_length, handles.axes2, 'left', 'pos', handles); end
    if (handles.show_ch7.Value) read_ch('ch7', 'busp - current', ch_data, [0 0.5 0], ch_length, handles.axes2, 'right', 'pos', handles); end
    if (handles.show_ch8.Value) read_ch('ch8', '1n - voltage', ch_data, 'blue', ch_length, handles.axes2, 'left', 'neg', handles); end
    if (handles.show_ch9.Value) read_ch('ch9', '1n - current', ch_data, 'red', ch_length, handles.axes2, 'right', 'neg', handles); end
    if (handles.show_ch10.Value) read_ch('cha', '2n - voltage', ch_data, 'cyan', ch_length, handles.axes2, 'left', 'neg', handles); end
    if (handles.show_ch11.Value) read_ch('chb', '2n - current', ch_data, 'magenta', ch_length, handles.axes2, 'right', 'neg', handles); end
    if (handles.show_ch12.Value) read_ch('chc', '3n - voltage', ch_data, '[0.2734 0.5078 0.7031]', ch_length, handles.axes2, 'left', 'neg', handles); end
    if (handles.show_ch13.Value) read_ch('chd', '3n - current', ch_data, 'black', ch_length, handles.axes2, 'right', 'neg', handles); end
    if (handles.show_ch14.Value) read_ch('che', 'busn - voltage', ch_data, [1 0.6 0], ch_length, handles.axes2, 'left', 'neg', handles); end
    if (handles.show_ch15.Value) read_ch('chf', 'busn - current', ch_data, [0 0.5 0], ch_length, handles.axes2, 'right', 'neg', handles); end
    %if (handles.show_ch_remote.Value) read_ch('chr', 'remote - current', ch_data, [0 0.5430 0.5430], ch_length, handles.axes2, 'right', 'neg', handles); end
        
    if (handles.show_ch0.Value) read_ch_lpf('fi0', '1p - voltage - filtered   .', ch_data, 'blue', ch_length, handles.axes2, 'left', 'pos', handles); end
    if (handles.show_ch1.Value) read_ch_lpf('fi1', '1p - current - filtered   .', ch_data, 'red', ch_length, handles.axes2, 'right', 'pos', handles); end
    if (handles.show_ch2.Value) read_ch_lpf('fi2', '2p - voltage - filtered   .', ch_data, 'cyan', ch_length, handles.axes2, 'left', 'pos', handles); end
    if (handles.show_ch3.Value) read_ch_lpf('fi3', '2p - current - filtered   .', ch_data, 'magenta', ch_length, handles.axes2, 'right', 'pos', handles); end
    if (handles.show_ch4.Value) read_ch_lpf('fi4', '3p - voltage - filtered   .', ch_data, '[0.2734 0.5078 0.7031]', ch_length, handles.axes2, 'left', 'pos', handles); end
    if (handles.show_ch5.Value) read_ch_lpf('fi5', '3p - current - filtered   .', ch_data, 'black', ch_length, handles.axes2, 'right', 'pos', handles); end
%     if (handles.show_ch6.Value) read_ch_lpf('fi6', 'p channel 7 - filtered', ch_data, [1 0.6 0], ch_length, handles.axes2, 'left', 'pos', handles); end
%     if (handles.show_ch7.Value) read_ch_lpf('fi7', 'p channel 8 - filtered', ch_data, [0 0.5 0], ch_length, handles.axes2, 'right', 'pos', handles); end
    if (handles.show_ch8.Value) read_ch_lpf('fi8', '1n - voltage - filtered   .', ch_data, 'blue', ch_length, handles.axes2, 'left', 'neg', handles); end
    if (handles.show_ch9.Value) read_ch_lpf('fi9', '1n - current - filtered   .', ch_data, 'red', ch_length, handles.axes2, 'right', 'neg', handles); end
    if (handles.show_ch10.Value) read_ch_lpf('fia', '2n - voltage - filtered   .', ch_data, 'cyan', ch_length, handles.axes2, 'left', 'neg', handles); end
    if (handles.show_ch11.Value) read_ch_lpf('fib', '2n - current - filtered   .', ch_data, 'magenta', ch_length, handles.axes2, 'right', 'neg', handles); end
    if (handles.show_ch12.Value) read_ch_lpf('fic', '3n - voltage - filtered   .', ch_data, '[0.2734 0.5078 0.7031]', ch_length, handles.axes2, 'left', 'neg', handles); end
    if (handles.show_ch13.Value) read_ch_lpf('fid', '3n - current - filtered   .', ch_data, 'black', ch_length, handles.axes2, 'right', 'neg', handles); end
%     if (handles.show_ch14.Value) read_ch_lpf('fie', 'n channel 7 - filtered', ch_data, [1 0.6 0], ch_length, handles.axes2, 'left', 'neg', handles); end
%     if (handles.show_ch15.Value) read_ch_lpf('fif', 'n channel 8 - filtered', ch_data, [0 0.5 0], ch_length, handles.axes2, 'right', 'neg', handles); end
    %if (handles.show_ch_remote.Value) read_ch_lpf('fir', 'remote - current - filtered   .', ch_data, [0 0.5430 0.5430], ch_length, handles.axes2, 'right', 'neg', handles); end

    handles.wait_text.BackgroundColor = 'g';
    handles.wait_text.String = 'OK';


function read_ch(ch_name, ch_plotname, ch_data, ch_color, ch_length, handles_axes, orientation, pole, handles)
    if(handles.only_filtered.Value==0)
    delete(instrfindall);
    SerialPort=edit_COM_Callback(handles);
    s = serial(SerialPort);
    set(s, 'InputBufferSize', 1024);
    set(s, 'BaudRate', 115200);
    set(s, 'Parity', 'none');
    set(s, 'DataBits', 8);
    set(s, 'StopBit', 1);
    set(s, 'Timeout', 1);
    s.ByteOrder = 'littleEndian';
    fopen(s);
    while(1)
         try fprintf(s,ch_name)
         catch
         end
        if(s.BytesAvailable~=0)
            break;
        end
    end
    step = 1;
    ch_data = [1:800]*0;
    while step < ch_length
        if(s.BytesAvailable~=0)        
            ch_data(step) = fread(s,1,'uint32');
            step = step+1;    
        end
    end
    pause(0.01);
    ch_data = ch_data/32767*20.48-10.24*2;
    axes(handles_axes);
    left_color = [0 0 0]; right_color = [0 0 0];
    set(handles_axes,'defaultAxesColorOrder',[left_color; right_color]);
    set(handles_axes,'Position',[0.31 0.662 0.518 0.271]);
    handles_axes.ActivePositionProperty = 'position';
    yyaxis(handles_axes,orientation)
    end
    if(pole=='pos')
        delete(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', ch_color, '-and', 'SizeData',20));
        if(handles.only_filtered.Value==0)
            scatter((0:length(ch_data)-1)/50, ch_data,20,'filled','MarkerFaceAlpha',1/10,'MarkerFaceColor',ch_color,'DisplayName', ch_plotname);
        end
    else
        delete(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', ch_color, '-and', 'SizeData',19));
        if(handles.only_filtered.Value==0)
            scatter((0:length(ch_data)-1)/50, ch_data, 19,'filled','MarkerFaceAlpha',1/10,'MarkerFaceColor',ch_color,'DisplayName', ch_plotname);
        end
    end
    grid on; hold on;
    xlabel('time [ms]');
    ylabel('voltage [V]');
    if strcmp(orientation,'left')
        voltage_upper_limit = voltage_upper_limit_Callback(handles);
        voltage_lower_limit = voltage_lower_limit_Callback(handles);
        ylim([voltage_lower_limit voltage_upper_limit]); 
        Ytick = get(handles.axes2, 'Ytick');
        distance = Ytick(2)-Ytick(1);
        newYTicks = [voltage_lower_limit:distance: voltage_upper_limit];  
        while length(newYTicks)>10
            distance = 2*distance;
            newYTicks = [voltage_lower_limit:distance:voltage_upper_limit];  
        end
        while length(newYTicks)<5
            distance = 0.5*distance;
            newYTicks = [voltage_lower_limit:distance:voltage_upper_limit];  
        end
        set(handles.axes2,'YTick',newYTicks,'YTickLabel',num2str(newYTicks'))
    else
        current_upper_limit = current_upper_limit_Callback(handles);
        current_lower_limit = current_lower_limit_Callback(handles);
        ylim([current_lower_limit current_upper_limit]);
        Ytick = get(handles.axes2, 'Ytick');
        distance = Ytick(2)-Ytick(1);
        newYTicks = [current_lower_limit:distance: current_upper_limit];  
        while length(newYTicks)>10
            distance = 2*distance;
            newYTicks = [current_lower_limit:distance:current_upper_limit];  
        end
        while length(newYTicks)<5
            distance = 0.5*distance;
            newYTicks = [current_lower_limit:distance:current_upper_limit];  
        end
        set(handles.axes2,'YTick',newYTicks,'YTickLabel',num2str(newYTicks'))
    end
    xlim([0 16]);
    legend('-DynamicLegend');
    legend('Location','northeastoutside');

function read_ch_lpf(ch_name, ch_plotname, ch_data, ch_color, ch_length, handles_axes, orientation, pole, handles)
    delete(instrfindall);
    SerialPort=edit_COM_Callback(handles);
    s = serial(SerialPort);
    set(s, 'InputBufferSize', 1024);
    set(s, 'BaudRate', 115200);
    set(s, 'Parity', 'none');
    set(s, 'DataBits', 8);
    set(s, 'StopBit', 1);
    set(s, 'Timeout', 1);
    fopen(s);
    while(1)
         try fprintf(s,ch_name)
         catch
         end
        if(s.BytesAvailable~=0)
            break;
        end
    end
    step = 1;
    ch_data = [1:800]*0;
    while step < ch_length
        if(s.BytesAvailable~=0)        
            ch_data(step) = fread(s,1,'uint32');
            step = step+1;    
        end
    end
    pause(0.01);
    ch_data = ch_data/32767*20.48-10.24*2;
    axes(handles_axes);
    left_color = [0 0 0]; right_color = [0 0 0];
    set(handles_axes,'defaultAxesColorOrder',[left_color; right_color]);
    set(handles_axes,'Position',[0.31 0.662 0.518 0.271]);
    handles_axes.ActivePositionProperty = 'position';
    yyaxis(handles_axes,orientation)
    if(pole=='pos')
        delete(findobj(gca, 'Type', 'Line','-and' ,'Color', ch_color, '-and','LineStyle','-', '-and','LineWidth', 1));
        delete(findobj(gca, 'Type', 'Line','-and' ,'Color', ch_color, '-and','LineStyle','-', '-and','LineWidth', 0.5));
        plot((0:length(ch_data)-1)/50, ch_data,'LineStyle','-','Marker','none','Color', ch_color, 'DisplayName', ch_plotname, 'LineWidth', 1);
    else
        delete(findobj(gca, 'Type', 'Line','-and' ,'Color', ch_color, '-and','LineStyle','-','-and','LineWidth',0.9));
        delete(findobj(gca, 'Type', 'Line','-and' ,'Color', ch_color, '-and','LineStyle','-','-and','LineWidth',0.5));
        plot((0:length(ch_data)-1)/50, ch_data,'LineStyle','-','Marker','none','Color', ch_color, 'DisplayName', ch_plotname, 'LineWidth', 0.9);
    end
    grid on; hold on;
    xlabel('time [ms]');
    ylabel('voltage [V]');
    if strcmp(orientation,'left')
        voltage_upper_limit = voltage_upper_limit_Callback(handles);
        voltage_lower_limit = voltage_lower_limit_Callback(handles);
        ylim([voltage_lower_limit voltage_upper_limit]); 
        Ytick = get(handles.axes2, 'Ytick');
        distance = Ytick(2)-Ytick(1);
        newYTicks = [voltage_lower_limit:distance: voltage_upper_limit];  
        while length(newYTicks)>10
            distance = 2*distance;
            newYTicks = [voltage_lower_limit:distance:voltage_upper_limit];  
        end
        while length(newYTicks)<5
            distance = 0.5*distance;
            newYTicks = [voltage_lower_limit:distance:voltage_upper_limit];  
        end
        set(handles.axes2,'YTick',newYTicks,'YTickLabel',num2str(newYTicks'))
    else
        current_upper_limit = current_upper_limit_Callback(handles);
        current_lower_limit = current_lower_limit_Callback(handles);
        ylim([current_lower_limit current_upper_limit]); 
        Ytick = get(handles.axes2, 'Ytick');
        distance = Ytick(2)-Ytick(1);
        newYTicks = [current_lower_limit:distance: current_upper_limit];  
        while length(newYTicks)>10
            distance = 2*distance;
            newYTicks = [current_lower_limit:distance:current_upper_limit];  
        end
        while length(newYTicks)<5
            distance = 0.5*distance;
            newYTicks = [current_lower_limit:distance:current_upper_limit];  
        end
        set(handles.axes2,'YTick',newYTicks,'YTickLabel',num2str(newYTicks'))
    end
    xlim([0 16]);
    legend('-DynamicLegend');
    legend('Location','northeastoutside');

% --- Executes on button press in show_ch0.
function show_ch0 = show_ch0_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
    show_ch0 = 1;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', 'blue','-and', 'SizeData',20),'MarkerFaceAlpha',1/10);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'blue','-and','LineStyle','none', '-and','LineWidth', 1),'LineStyle','-');
else
    show_ch0 = 0;
	axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', 'blue','-and', 'SizeData',20),'MarkerFaceAlpha',0);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'blue','-and','LineStyle','-', '-and','LineWidth', 1),'LineStyle','none');
end

% --- Executes on button press in show_ch1.
function show_ch1 = show_ch1_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	show_ch1 = 1;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', 'red','-and', 'SizeData',20),'MarkerFaceAlpha',1/10);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'red','-and','LineStyle','none', '-and','LineWidth', 1),'LineStyle','-');
else
	show_ch1 = 0;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', 'red','-and', 'SizeData',20),'MarkerFaceAlpha',0);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'red','-and','LineStyle','-', '-and','LineWidth', 1),'LineStyle','none');
end

% --- Executes on button press in show_ch2.
function show_ch2 = show_ch2_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	show_ch2 = 1;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', 'cyan','-and', 'SizeData',20),'MarkerFaceAlpha',1/10);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'cyan','-and','LineStyle','none', '-and','LineWidth', 1),'LineStyle','-');
else
    show_ch2 = 0;
	axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', 'cyan','-and', 'SizeData',20),'MarkerFaceAlpha',0);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'cyan','-and','LineStyle','-', '-and','LineWidth', 1),'LineStyle','none');
end

% --- Executes on button press in show_ch3.
function show_ch3 = show_ch3_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	show_ch3 = 1;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', 'magenta','-and', 'SizeData',20),'MarkerFaceAlpha',1/10);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'magenta','-and','LineStyle','none', '-and','LineWidth', 1),'LineStyle','-');
else
	show_ch3 = 0;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', 'magenta','-and', 'SizeData',20),'MarkerFaceAlpha',0);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'magenta','-and','LineStyle','-', '-and','LineWidth', 1),'LineStyle','none');
end

% --- Executes on button press in show_ch4.
function show_ch4 = show_ch4_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	show_ch4 = 1;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', [0.2734 0.5078 0.7031],'-and', 'SizeData',20),'MarkerFaceAlpha',1/10);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [0.2734 0.5078 0.7031],'-and','LineStyle','none', '-and','LineWidth', 1),'LineStyle','-');
else
	show_ch4 = 0;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', [0.2734 0.5078 0.7031],'-and', 'SizeData',20),'MarkerFaceAlpha',0);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [0.2734 0.5078 0.7031],'-and','LineStyle','-', '-and','LineWidth', 1),'LineStyle','none');
end

% --- Executes on button press in show_ch5.
function show_ch5 = show_ch5_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	show_ch5 = 1;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', 'black','-and', 'SizeData',20),'MarkerFaceAlpha',1/10);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'black','-and','LineStyle','none', '-and','LineWidth', 1),'LineStyle','-');
else
	show_ch5 = 1;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', 'black','-and', 'SizeData',20),'MarkerFaceAlpha',0);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'black','-and','LineStyle','-', '-and','LineWidth', 1),'LineStyle','none');
end

% --- Executes on button press in show_ch6.
function show_ch6 = show_ch6_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	show_ch6 = 1;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', [1 0.6 0],'-and', 'SizeData',20),'MarkerFaceAlpha',1/10);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [1 0.6 0],'-and','LineStyle','none', '-and','LineWidth', 1),'LineStyle','-');
else
	show_ch6 = 0;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', [1 0.6 0],'-and', 'SizeData',20),'MarkerFaceAlpha',0);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [1 0.6 0],'-and','LineStyle','-', '-and','LineWidth', 1),'LineStyle','none');
end
% --- Executes on button press in show_ch7.
function show_ch7 = show_ch7_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	show_ch7 = 1;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', [0 0.5 0],'-and', 'SizeData',20),'MarkerFaceAlpha',1/10);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [0 0.5 0],'-and','LineStyle','none', '-and','LineWidth', 1),'LineStyle','-');
else
	show_ch7 = 0;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', [0 0.5 0],'-and', 'SizeData',20),'MarkerFaceAlpha',0);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [0 0.5 0],'-and','LineStyle','-', '-and','LineWidth', 1),'LineStyle','none');
end

% --- Executes on button press in show_ch8.
function show_ch8 = show_ch8_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	show_ch8 = 1;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', 'blue','-and', 'SizeData',19),'MarkerFaceAlpha',1/10);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'blue','-and','LineStyle','none','LineWidth', 0.9),'LineStyle','-');
else
	show_ch8 = 0;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', 'blue','-and', 'SizeData',19),'MarkerFaceAlpha',0);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'blue','-and','LineStyle','-','LineWidth', 0.9),'LineStyle','none');
end

% --- Executes on button press in show_ch9.
function show_ch9 = show_ch9_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	show_ch9 = 1;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', 'red','-and', 'SizeData',19),'MarkerFaceAlpha',1/10);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'red','-and','LineStyle','none','LineWidth', 0.9),'LineStyle','-');
else
	show_ch9 = 0;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', 'red','-and', 'SizeData',19),'MarkerFaceAlpha',0);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'red','-and','LineStyle','-','LineWidth', 0.9),'LineStyle','none');
end

% --- Executes on button press in show_ch10.
function show_ch10 = show_ch10_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	show_ch10 = 1;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', 'cyan','-and', 'SizeData',19),'MarkerFaceAlpha',1/10);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'cyan','-and','LineStyle','none','LineWidth', 0.9),'LineStyle','-');
else
	show_ch10 = 0;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', 'cyan','-and', 'SizeData',19),'MarkerFaceAlpha',0);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'cyan','-and','LineStyle','-','LineWidth', 0.9),'LineStyle','none');
end

% --- Executes on button press in show_ch11.
function show_ch11 = show_ch11_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	show_ch11 = 1;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', 'magenta','-and', 'SizeData',19),'MarkerFaceAlpha',1/10);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'magenta','-and','LineStyle','none','LineWidth', 0.9),'LineStyle','-');
else
	show_ch11 = 0;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', 'magenta','-and', 'SizeData',19),'MarkerFaceAlpha',0);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'magenta','-and','LineStyle','-','LineWidth', 0.9),'LineStyle','none');
end

% --- Executes on button press in show_ch12.
function show_ch12 = show_ch12_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	show_ch12 = 1;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', [0.2734 0.5078 0.7031],'-and', 'SizeData',19),'MarkerFaceAlpha',1/10);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [0.2734 0.5078 0.7031],'-and','LineStyle','none','LineWidth', 0.9),'LineStyle','-');
else
	show_ch12 = 0;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', [0.2734 0.5078 0.7031],'-and', 'SizeData',19),'MarkerFaceAlpha',0);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [0.2734 0.5078 0.7031],'-and','LineStyle','-','LineWidth', 0.9),'LineStyle','none');
end

% --- Executes on button press in show_ch13.
function show_ch13 = show_ch13_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	show_ch13 = 1;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', 'black','-and', 'SizeData',19),'MarkerFaceAlpha',1/10);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'black','-and','LineStyle','none','LineWidth', 0.9),'LineStyle','-');
else
	show_ch13 = 0;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', 'black','-and', 'SizeData',19),'MarkerFaceAlpha',0);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'black','-and','LineStyle','-','LineWidth', 0.9),'LineStyle','none');
end

% --- Executes on button press in show_ch14.
function show_ch14 = show_ch14_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	show_ch14 = 1;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', [1 0.6 0],'-and', 'SizeData',19),'MarkerFaceAlpha',1/10);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [1 0.6 0],'-and','LineStyle','none','LineWidth', 0.9),'LineStyle','-');
else
	show_ch14 = 0;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', [1 0.6 0],'-and', 'SizeData',19),'MarkerFaceAlpha',0);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [1 0.6 0],'-and','LineStyle','-','LineWidth', 0.9),'LineStyle','none');
end
% --- Executes on button press in show_ch15.
function show_ch15 = show_ch15_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	show_ch15 = 1;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', [0 0.5 0],'-and', 'SizeData',19),'MarkerFaceAlpha',1/10);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [0 0.5 0],'-and','LineStyle','none','LineWidth', 0.9),'LineStyle','-');
else
	show_ch15 = 0;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', [0 0.5 0],'-and', 'SizeData',19),'MarkerFaceAlpha',0);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [0 0.5 0],'-and','LineStyle','-','LineWidth', 0.9),'LineStyle','none');
end
% --- Executes on button press in show_ch_remote.
function show_ch_remote = show_ch_remote_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	show_ch_remote = 1;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', [0 0.5430 0.5430],'-and', 'SizeData',19),'MarkerFaceAlpha',1/10);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [0 0.5430 0.5430],'-and','LineStyle','none','LineWidth', 0.9),'LineStyle','-');
else
	show_ch_remote = 0;
    axes(handles.axes2);
    set(findobj(gca, 'Type', 'Scatter', '-and','MarkerFaceColor', [0 0.5430 0.5430],'-and', 'SizeData',19),'MarkerFaceAlpha',0);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [0 0.5430 0.5430],'-and','LineStyle','-','LineWidth', 0.9),'LineStyle','none');
end




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% READ ALGORITHMS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% //alg1: dv/dt
% //alg2: dv/dt + current direction
% //alg3: TW extraction (local)
% //alg4: undervoltage
% //alg5: overcurrent
% //alg6: TW + current directional (double-ended)
% //alg7: TW + current differential (double-ended)
% //alg8: Busbar
% //alg9: backup: breaker failure
% //alg10:backup (local)
% --- Executes on button press in update_algorithms_pos1.
function update_algorithms_pos1_Callback(hObject, eventdata, handles)
    handles.wait_text.BackgroundColor = 'r';
    handles.wait_text.String = 'WAIT';
    ch_length = 800;
    ch_data = (0:1:ch_length).*0;
    if (handles.alg1.Value) read_alg_crit('c11', 'dv/dt', ch_data, 'blue', ch_length, handles.axes3, 'left',handles); end
    if (handles.alg2.Value) read_alg_crit('c12', ['dv/dt + current direction:' newline 'p.u. increase compared to' newline 'current pre dv/dt alarm'], ch_data, 'red', ch_length, handles.axes3, 'right',handles);end
    if (handles.alg3.Value) read_alg_crit('c13', 'TW extraction (local)', ch_data, 'cyan', ch_length, handles.axes3, 'left',handles);end
    if (handles.alg4.Value) read_alg_crit('c14', 'p.u. undervoltage', ch_data, 'magenta', ch_length, handles.axes3, 'right',handles);end
    if (handles.alg5.Value) read_alg_crit('c15', 'p.u. overcurrent', ch_data, '[0.2734 0.5078 0.7031]', ch_length, handles.axes3, 'right',handles);end
%     if (handles.alg6.Value) read_alg_crit('c16', 'TW + current directional (double-ended)', ch_data, 'black', ch_length, handles.axes3, 'left', handles);end
%     if (handles.alg7.Value) read_alg_crit('c17', 'current differential (double-ended)', ch_data, '[1 0.6 0]', ch_length, handles.axes3, 'right', handles);end
    if (handles.alg8.Value) read_alg_crit('c18', 'Busbar', ch_data, '[0 0.5 0]', ch_length, handles.axes3, 'right', handles);end
%     if (handles.alg9.Value) read_alg_crit('c19', 'Backup: breaker failure', ch_data, '[0.75 0.75 0.75]', ch_length, handles.axes3, 'right', handles);end
%     if (handles.alg10.Value) read_alg_crit('c1a', 'Backup (local)', ch_data, '[0 0.543 0.543]', ch_length, handles.axes3, 'left', handles);end
    handles.wait_text.BackgroundColor = 'g';
    handles.wait_text.String = 'OK';


function update_algorithms_pos2_Callback(hObject, eventdata, handles)
    handles.wait_text.BackgroundColor = 'r';
    handles.wait_text.String = 'WAIT';
    ch_length = 800;
    ch_data = (0:1:ch_length).*0;
    if (handles.alg1.Value)read_alg_crit('c21', 'dv/dt', ch_data, 'blue', ch_length, handles.axes3, 'left',handles); end
    if (handles.alg2.Value)read_alg_crit('c22', ['dv/dt + current direction:' newline 'p.u. increase compared to' newline 'current pre dv/dt alarm'], ch_data, 'red', ch_length, handles.axes3, 'right',handles); end
    if (handles.alg3.Value)read_alg_crit('c23', 'TW extraction (local)', ch_data, 'cyan', ch_length, handles.axes3, 'left',handles); end
    if (handles.alg4.Value)read_alg_crit('c24', 'p.u. undervoltage', ch_data, 'magenta', ch_length, handles.axes3, 'right',handles); end
    if (handles.alg5.Value)read_alg_crit('c25', 'p.u. overcurrent', ch_data, '[0.2734 0.5078 0.7031]', ch_length, handles.axes3, 'right',handles); end
%     if (handles.alg6.Value)read_alg_crit('c26', 'TW + current directional (double-ended)', ch_data, 'black', ch_length, handles.axes3, 'left', handles); end
%     if (handles.alg7.Value)read_alg_crit('c27', 'TW + current differential (double-ended)', ch_data, '[1 0.6 0]', ch_length, handles.axes3, 'left', handles); end
    if (handles.alg8.Value)read_alg_crit('c28', 'Busbar', ch_data, '[0 0.5 0]', ch_length, handles.axes3, 'right', handles); end
%     if (handles.alg9.Value)read_alg_crit('c29', 'Backup: breaker failure', ch_data, '[0.75 0.75 0.75]', ch_length, handles.axes3, 'right', handles); end
%     if (handles.alg10.Value)read_alg_crit('c2a', 'Backup (local)', ch_data, '[0 0.543 0.543]', ch_length, handles.axes3, 'left', handles); end
    handles.wait_text.BackgroundColor = 'g';
    handles.wait_text.String = 'OK';

function update_algorithms_pos3_Callback(hObject, eventdata, handles)
    handles.wait_text.BackgroundColor = 'r';
    handles.wait_text.String = 'WAIT';
    ch_length = 800;
    ch_data = (0:1:ch_length).*0;
    if (handles.alg1.Value) read_alg_crit('c31', 'dv/dt', ch_data, 'blue', ch_length, handles.axes3, 'left',handles); end
    if (handles.alg2.Value) read_alg_crit('c32', ['dv/dt + current direction:' newline 'p.u. increase compared to' newline 'current pre dv/dt alarm'], ch_data, 'red', ch_length, handles.axes3, 'right',handles); end
    if (handles.alg3.Value) read_alg_crit('c33', 'TW extraction (local)', ch_data, 'cyan', ch_length, handles.axes3, 'left',handles); end
    if (handles.alg4.Value) read_alg_crit('c34', 'p.u. undervoltage', ch_data, 'magenta', ch_length, handles.axes3, 'right',handles); end
    if (handles.alg5.Value) read_alg_crit('c35', 'p.u. overcurrent', ch_data, '[0.2734 0.5078 0.7031]', ch_length, handles.axes3, 'right',handles); end
%     if (handles.alg6.Value) read_alg_crit('c36', 'TW + current directional (double-ended)', ch_data, 'black', ch_length, handles.axes3, 'left', handles); end
%     if (handles.alg7.Value) read_alg_crit('c37', 'TW + current differential (double-ended)', ch_data, [1 0.6 0], ch_length, handles.axes3, 'left', handles); end
    if (handles.alg8.Value) read_alg_crit('c38', 'Busbar', ch_data, '[0 0.5 0]', ch_length, handles.axes3, 'right', handles); end
%     if (handles.alg9.Value) read_alg_crit('c39', 'Backup: breaker failure', ch_data, '[0.75 0.75 0.75]', ch_length, handles.axes3, 'right', handles); end
%     if (handles.alg10.Value) read_alg_crit('c3a', 'Backup (local)', ch_data, '[0 0.543 0.543]', ch_length, handles.axes3, 'left', handles); end
    handles.wait_text.BackgroundColor = 'g';
    handles.wait_text.String = 'OK';


% --- Executes on button press in update_algorithms_neg1.
function update_algorithms_neg1_Callback(hObject, eventdata, handles)
    handles.wait_text.BackgroundColor = 'r';
    handles.wait_text.String = 'WAIT';
    ch_length = 800;
    ch_data = (0:1:ch_length).*0;
    if (handles.alg1.Value) read_alg_crit('c41', 'dv/dt', ch_data, 'blue', ch_length, handles.axes3, 'left',handles); end
    if (handles.alg2.Value) read_alg_crit('c42', ['dv/dt + current direction:' newline 'p.u. increase compared to' newline 'current pre dv/dt alarm'], ch_data, 'red', ch_length, handles.axes3, 'right',handles); end    
    if (handles.alg3.Value) read_alg_crit('c43', 'TW extraction (local)', ch_data, 'cyan', ch_length, handles.axes3, 'left',handles); end    
    if (handles.alg4.Value) read_alg_crit('c44', 'p.u. undervoltage', ch_data, 'magenta', ch_length, handles.axes3, 'right',handles); end
    if (handles.alg5.Value) read_alg_crit('c45', 'p.u. overcurrent', ch_data, '[0.2734 0.5078 0.7031]', ch_length, handles.axes3, 'right',handles); end    
%     if (handles.alg6.Value) read_alg_crit('c46', 'TW + current directional (double-ended)', ch_data, 'black', ch_length, handles.axes3, 'left', handles); end
%     if (handles.alg7.Value) read_alg_crit('c47', 'TW + current differential (double-ended)', ch_data, '[1 0.6 0]', ch_length, handles.axes3, 'left', handles);end
    if (handles.alg8.Value) read_alg_crit('c48', 'Busbar', ch_data, '[0 0.5 0]', ch_length, handles.axes3, 'right', handles);end
%     if (handles.alg9.Value) read_alg_crit('c49', 'Backup: breaker failure', ch_data, '[0.75 0.75 0.75]', ch_length, handles.axes3, 'right', handles);end
%     if (handles.alg10.Value) read_alg_crit('c4a', 'Backup (local)', ch_data, '[0 0.543 0.543]', ch_length, handles.axes3, 'left', handles);end
    handles.wait_text.BackgroundColor = 'g';
    handles.wait_text.String = 'OK';

% --- Executes on button press in update_algorithms_pos2.
function update_algorithms_neg2_Callback(hObject, eventdata, handles)
    handles.wait_text.BackgroundColor = 'r';
    handles.wait_text.String = 'WAIT';
    ch_length = 800;
    ch_data = (0:1:ch_length).*0;
    if (handles.alg1.Value) read_alg_crit('c51', 'dv/dt', ch_data, 'blue', ch_length, handles.axes3, 'left',handles); end
    if (handles.alg2.Value) read_alg_crit('c52', ['dv/dt + current direction:' newline 'p.u. increase compared to' newline 'current pre dv/dt alarm'], ch_data, 'red', ch_length, handles.axes3, 'right',handles); end    
    if (handles.alg3.Value) read_alg_crit('c53', 'TW extraction (local)', ch_data, 'cyan', ch_length, handles.axes3, 'left',handles); end    
    if (handles.alg4.Value) read_alg_crit('c54', 'p.u. undervoltage', ch_data, 'magenta', ch_length, handles.axes3, 'right',handles); end
    if (handles.alg5.Value) read_alg_crit('c55', 'p.u. overcurrent', ch_data, '[0.2734 0.5078 0.7031]', ch_length, handles.axes3, 'right',handles); end    
%     if (handles.alg6.Value) read_alg_crit('c56', 'TW + current directional (double-ended)', ch_data, 'black', ch_length, handles.axes3, 'left', handles); end
%     if (handles.alg7.Value) read_alg_crit('c57', 'TW + current differential (double-ended)', ch_data, '[1 0.6 0]', ch_length, handles.axes3, 'left', handles); end
    if (handles.alg8.Value) read_alg_crit('c58', 'Busbar', ch_data, '[0 0.5 0]', ch_length, handles.axes3, 'right', handles); end
%     if (handles.alg9.Value) read_alg_crit('c59', 'Backup: breaker failure', ch_data, '[0.75 0.75 0.75]', ch_length, handles.axes3, 'right', handles); end
%     if (handles.alg10.Value) read_alg_crit('c5a', 'Backup (local)', ch_data, '[0 0.543 0.543]', ch_length, handles.axes3, 'left', handles); end
    handles.wait_text.BackgroundColor = 'g';
    handles.wait_text.String = 'OK';

% --- Executes on button press in update_algorithms_pos3.
function update_algorithms_neg3_Callback(hObject, eventdata, handles)
    handles.wait_text.BackgroundColor = 'r';
    handles.wait_text.String = 'WAIT';    
    ch_length = 800;
    ch_data = (0:1:ch_length).*0;    
    if (handles.alg1.Value) read_alg_crit('c61', 'dv/dt', ch_data, 'blue', ch_length, handles.axes3, 'left',handles); end
    if (handles.alg2.Value) read_alg_crit('c62', ['dv/dt + current direction:' newline 'p.u. increase compared to' newline 'current pre dv/dt alarm'], ch_data, 'red', ch_length, handles.axes3, 'right',handles); end    
    if (handles.alg3.Value) read_alg_crit('c63', 'TW extraction (local)', ch_data, 'cyan', ch_length, handles.axes3, 'left',handles); end    
    if (handles.alg4.Value) read_alg_crit('c64', 'p.u. undervoltage', ch_data, 'magenta', ch_length, handles.axes3, 'right',handles); end
    if (handles.alg5.Value) read_alg_crit('c65', 'p.u. overcurrent', ch_data, '[0.2734 0.5078 0.7031]', ch_length, handles.axes3, 'right',handles); end    
%     if (handles.alg6.Value) read_alg_crit('c66', 'TW + current directional (double-ended)', ch_data, 'black', ch_length, handles.axes3, 'left', handles); end
%     if (handles.alg7.Value) read_alg_crit('c67', 'TW + current differential (double-ended)', ch_data, [1 0.6 0], ch_length, handles.axes3, 'left', handles); end
    if (handles.alg8.Value) read_alg_crit('c68', 'Busbar', ch_data, '[0 0.5 0]', ch_length, handles.axes3, 'right', handles); end
%     if (handles.alg9.Value) read_alg_crit('c69', 'Backup: breaker failure', ch_data, '[0.75 0.75 0.75]', ch_length, handles.axes3, 'right', handles); end
%     if (handles.alg10.Value) read_alg_crit('c6a', 'Backup (local)', ch_data, '[0 0.543 0.543]', ch_length, handles.axes3, 'left', handles); end
    handles.wait_text.BackgroundColor = 'g';
    handles.wait_text.String = 'OK';


function read_alg_crit(ch_name, ch_plotname, ch_data, ch_color, ch_length, handles_axes, orientation, handles)
    delete(instrfindall);
    SerialPort=edit_COM_Callback(handles);
    s = serial(SerialPort);
    set(s, 'InputBufferSize', 1024);
    set(s, 'BaudRate', 115200);
    set(s, 'Parity', 'none');
    set(s, 'DataBits', 8);
    set(s, 'StopBit', 1);
    set(s, 'Timeout', 1);
    fopen(s);
    while(1)
         try fprintf(s,ch_name)
         catch
         end
        if(s.BytesAvailable~=0)
            break;
        end
    end
    step = 1;
    while step < ch_length
        if(s.BytesAvailable~=0)        
            ch_data(step) = fread(s,1,'uint32');
            step = step+1;    
        end
    end
    pu_factor_voltage = edit_pu_factor_voltage_Callback(handles);
    pu_factor_current = edit_pu_factor_current_Callback(handles);
    scale_factor_voltage = edit_scale_factor_voltage_Callback(handles);
    scale_factor_current = edit_scale_factor_current_Callback(handles);
    if strcmp(ch_name,'c12') || strcmp(ch_name,'c22') || strcmp(ch_name,'c32') || strcmp(ch_name,'c42') || strcmp(ch_name,'c52') || strcmp(ch_name,'c62')%dv/dt + current direciton change
        %get overcurrent in Ampere (|current-alarm_current|)
        ch_data = ch_data/32767*20.48;
        overcurrent = ch_data;        
        %get normal current
        if     strcmp(ch_name,'c12')            ch_name = 'fi1';
        elseif strcmp(ch_name,'c22')            ch_name = 'fi3';
        elseif strcmp(ch_name,'c32')            ch_name = 'fi5';
        elseif strcmp(ch_name,'c42')            ch_name = 'fi9';
        elseif strcmp(ch_name,'c52')            ch_name = 'fib';
        elseif strcmp(ch_name,'c62')            ch_name = 'fid';
        end
     
        delete(instrfindall);
        SerialPort=edit_COM_Callback(handles);
        s = serial(SerialPort);
        set(s, 'InputBufferSize', 1024);
        set(s, 'BaudRate', 115200);
        set(s, 'Parity', 'none');
        set(s, 'DataBits', 8);
        set(s, 'StopBit', 1);
        set(s, 'Timeout', 1);
        fopen(s);
        while(1)
             try fprintf(s,ch_name)
             catch
             end
            if(s.BytesAvailable~=0)
                break;
            end
        end
        step = 1;
        while step < ch_length
            if(s.BytesAvailable~=0)        
                current(step) = fread(s,1,'uint32');
                step = step+1;    
            end
        end
        current = current/32767*20.48-10.24*2;
        
        %find first non-zero overcurrent. This equals to pre-fault current
        current_prefault = abs(current(min(find(overcurrent))));
        %scale plot to be in p.u.
        try
            ch_data = overcurrent./current_prefault;
        catch
            ch_data = overcurrent.*0;
        end
        
        scaled_pu = pu_factor_current/scale_factor_current;
        ch_data = (ch_data./scaled_pu) + 1; %scaling, +1 to make it fit in p.u. scale 
        
    elseif strcmp(ch_name,'c14') || strcmp(ch_name,'c24') || strcmp(ch_name,'c34') %undervoltage pos pole
        ch_data = ch_data/32767*20.48 - 2*10.24;
        scaled_pu = pu_factor_voltage/scale_factor_voltage;
        ch_data = 1-(scaled_pu-ch_data)/scaled_pu; 
    elseif strcmp(ch_name,'c44') || strcmp(ch_name,'c54') || strcmp(ch_name,'c64') %undervoltage neg pole
        ch_data = ch_data/32767*20.48 - 2*10.24;
        scaled_pu = pu_factor_voltage/scale_factor_voltage;
        ch_data = 1-(scaled_pu-ch_data)/scaled_pu;
        ch_data = -ch_data;
    elseif strcmp(ch_name,'c15') || strcmp(ch_name,'c25') || strcmp(ch_name,'c35')||strcmp(ch_name,'c45') || strcmp(ch_name,'c55') || strcmp(ch_name,'c65') %overcurrent
        ch_data = ch_data/32767*20.48 - 2*10.24;
        ch_data = abs(ch_data);      
    elseif strcmp(ch_name,'c18') || strcmp(ch_name,'c28') || strcmp(ch_name,'c38')||strcmp(ch_name,'c48') || strcmp(ch_name,'c58') || strcmp(ch_name,'c68') %busbar
        ch_data = (ch_data/32767*20.48);
        scaled_pu = pu_factor_current/scale_factor_current;
        ch_data = (ch_data-scaled_pu)/scaled_pu +1; 
    else
        ch_data = ch_data/32767*20.48;
    end
    axes(handles_axes);
    left_color = [0 0 0]; right_color = [0 0 0];
    set(handles_axes,'defaultAxesColorOrder',[left_color; right_color]);
    set(handles_axes,'Position',[0.31 0.321 0.518 0.271]);
    handles_axes.ActivePositionProperty = 'position';
    yyaxis(handles_axes,orientation);
    delete(findobj(gca, 'Type', 'Line','-and','LineStyle','-','-and', 'Color', ch_color));
    plot((0:length(ch_data)-1)/50, ch_data,'LineStyle','-','LineWidth',0.5,'Marker','none','Color', ch_color, 'DisplayName', ch_plotname);    
    grid on; hold on;
    xlabel('time [ms]');
    if strcmp(orientation,'left')        
        scale_factor_voltage = edit_scale_factor_voltage_Callback(handles);
        upper_limit = left_limit(handles)/scale_factor_voltage;
        lower_limit = left_limit_lower(handles)/scale_factor_voltage;
        ylim([lower_limit upper_limit]);
        Ytick = get(handles.axes3, 'Ytick');
        distance = Ytick(2)-Ytick(1);
        newYTicks = [lower_limit:distance: upper_limit];  
        while length(newYTicks)>10
            distance = 2*distance;
            newYTicks = [lower_limit:distance:upper_limit];  
        end
        while length(newYTicks)<5
            distance = 0.5*distance;
            newYTicks = [lower_limit:distance:upper_limit];  
        end
        set(handles.axes3,'YTick',newYTicks,'YTickLabel',num2str(newYTicks'*scale_factor_voltage))
        ylabel('[kV/20us] or [kV]');        
    else
        ylim([0 10]);
        ylabel('[p.u.]');
    end
    xlim([0 16]);
    legend('-DynamicLegend');
    legend('Location','northeastoutside');


    
    
    
    
    
    
    
    
    
    
    
    
    
    
function turn_alg_onoff(onoff, handles)
    handles.wait_text.BackgroundColor = 'r';
    handles.wait_text.String = 'WAIT'; 
    delete(instrfindall);
    SerialPort=edit_COM_Callback(handles);
    s = serial(SerialPort);
    set(s, 'InputBufferSize', 1024);
    set(s, 'BaudRate', 115200);
    set(s, 'Parity', 'none');
    set(s, 'DataBits', 8);
    set(s, 'StopBit', 1);
    set(s, 'Timeout', 1);
    fopen(s);
    while(1)
         try fprintf(s,onoff)
         catch
         end
        if(s.BytesAvailable~=0)
            break;
        end
    end
    while(s.BytesAvailable~=0)
        dummy = fread(s,1,'uint32');
    end    
    handles.wait_text.BackgroundColor = 'g';
    handles.wait_text.String = 'OK';

    
    % --- Executes on button press in alg1.
function alg1_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	axes(handles.axes3);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'blue','-and','linewidth',0.5,'-and','LineStyle','none'),'LineStyle','-');
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'blue','-and','linewidth',1.5,'-and','LineStyle','none'),'LineStyle',':');
    turn_alg_onoff('on1', handles);
else
	axes(handles.axes3);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'blue'),'LineStyle','none');
    turn_alg_onoff('of1', handles);
end


% --- Executes on button press in alg2.
function alg2_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	axes(handles.axes3);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'red','-and','linewidth',0.5,'-and','LineStyle','none'),'LineStyle','-');
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'red','-and','linewidth',1.5,'-and','LineStyle','none'),'LineStyle',':');
    turn_alg_onoff('on2', handles);
else
	axes(handles.axes3);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'red'),'LineStyle','none');
    turn_alg_onoff('of2', handles);
end

% --- Executes on button press in alg3.
function alg3_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	axes(handles.axes3);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'cyan','-and','linewidth',0.5,'-and','LineStyle','none'),'LineStyle','-');
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'cyan','-and','linewidth',1.5,'-and','LineStyle','none'),'LineStyle',':');
    turn_alg_onoff('on3', handles);
else
	axes(handles.axes3);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'cyan'),'LineStyle','none');
    turn_alg_onoff('of3', handles);
end

% --- Executes on button press in alg4.
function alg4_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	axes(handles.axes3);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'magenta','-and','linewidth',0.5,'-and','LineStyle','none'),'LineStyle','-');
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'magenta','-and','linewidth',1.5,'-and','LineStyle','none'),'LineStyle',':');
    turn_alg_onoff('on4', handles);
else
	axes(handles.axes3);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'magenta'),'LineStyle','none');
    turn_alg_onoff('of4', handles);
end

% --- Executes on button press in alg5.
function alg5_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	axes(handles.axes3);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [0.2734 0.5078 0.7031],'-and','linewidth',0.5,'-and','LineStyle','none'),'LineStyle','-');
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [0.2734 0.5078 0.7031],'-and','linewidth',1.5,'-and','LineStyle','none'),'LineStyle',':');
    turn_alg_onoff('on5', handles);
else
	axes(handles.axes3);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [0.2734 0.5078 0.7031]),'LineStyle','none');
    turn_alg_onoff('of5', handles);
end

% --- Executes on button press in alg6.
function alg6_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	axes(handles.axes3);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'black','-and','linewidth',0.5,'-and','LineStyle','none'),'LineStyle','-');
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'black','-and','linewidth',1.5,'-and','LineStyle','none'),'LineStyle',':');
    turn_alg_onoff('on6', handles);
else
	axes(handles.axes3);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', 'black'),'LineStyle','none');
    turn_alg_onoff('of6', handles);
end

% --- Executes on button press in alg7.
function alg7_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	axes(handles.axes3);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [1 0.6 0],'-and','linewidth',0.5,'-and','LineStyle','none'),'LineStyle','-');
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [1 0.6 0],'-and','linewidth',1.5,'-and','LineStyle','none'),'LineStyle',':');
    turn_alg_onoff('on7', handles);
else
	axes(handles.axes3);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [1 0.6 0]),'LineStyle','none');
    turn_alg_onoff('of7', handles);
end

% --- Executes on button press in alg8.
function alg8_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	axes(handles.axes3);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [0 0.5 0],'-and','linewidth',0.5,'-and','LineStyle','none'),'LineStyle','-');
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [0 0.5 0],'-and','linewidth',1.5,'-and','LineStyle','none'),'LineStyle',':');
    turn_alg_onoff('on8', handles);
else
	axes(handles.axes3);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [0 0.5 0]),'LineStyle','none');
    turn_alg_onoff('of8', handles);
end

% --- Executes on button press in alg9.
function alg9_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	axes(handles.axes3);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [0.75 0.75 0.75],'-and','linewidth',0.5,'-and','LineStyle','none'),'LineStyle','-');
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [0.75 0.75 0.75],'-and','linewidth',1.5,'-and','LineStyle','none'),'LineStyle',':');
    turn_alg_onoff('on9', handles);
else
	axes(handles.axes3);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [0.75 0.75 0.75]),'LineStyle','none');
    turn_alg_onoff('of9', handles);
end

% --- Executes on button press in alg10.
function alg10_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
	axes(handles.axes3);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [0 0.543 0.543],'-and','LineStyle','none'),'LineStyle','-');
    turn_alg_onoff('ona', handles);
else
	axes(handles.axes3);
    set(findobj(gca, 'Type', 'Line','-and' ,'Color', [0 0.543 0.543],'-and','LineStyle','-'),'LineStyle','none');
    turn_alg_onoff('ofa', handles);
end






% --- Executes on button press in update_trip.
function update_trip_Callback(handles)
    axes(handles.axes2);
    delete(findobj(gca, 'Type', 'line', 'Color', 'green'));
    step = 0.4*800;
    left_color = [0 0 0]; right_color = [0 0 0];
    set(handles.axes2,'defaultAxesColorOrder',[left_color; right_color]);
    set(handles.axes2,'Position',[0.31 0.662 0.518 0.271]);
    handles.axes2.ActivePositionProperty = 'position';
    yyaxis left;
    plot(((step-1):step)/50, [-20 20],'g-','linewidth',2, 'DisplayName', 'trip');
    grid on; hold on;
    xlabel('time [ms]');
    legend('-DynamicLegend');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% SET THRESHOLDS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function pos1_alg1_thres = pos1_alg1_thres_Callback(hObject, eventdata, handles)
    pos1_alg1_thres = str2double(get(handles.pos1_alg1_thres,'string'));
    plot_thres(pos1_alg1_thres, '    threshold', 'blue', 'left', handles);
    send_thres(pos1_alg1_thres, 1, handles);
    
function pos1_alg2_thres = pos1_alg2_thres_Callback(hObject, eventdata, handles)
    pos1_alg2_thres = str2double(get(handles.pos1_alg2_thres,'string'));
    plot_thres(pos1_alg2_thres, '    threshold', 'red', 'right', handles);
    send_thres(pos1_alg2_thres, 2, handles);
    
function pos1_alg3_thres = pos1_alg3_thres_Callback(hObject, eventdata, handles)
    pos1_alg3_thres = str2double(get(handles.pos1_alg3_thres,'string'));
    plot_thres(pos1_alg3_thres, '    threshold', 'cyan', 'left', handles);
    send_thres(pos1_alg3_thres, 3, handles);
    
function pos1_alg4_thres = pos1_alg4_thres_Callback(hObject, eventdata, handles)
    pos1_alg4_thres = str2double(get(handles.pos1_alg4_thres,'string'));
    plot_thres(pos1_alg4_thres, '    threshold', 'magenta', 'right', handles); 
    send_thres(pos1_alg4_thres, 4, handles);
    
function pos1_alg5_thres = pos1_alg5_thres_Callback(hObject, eventdata, handles)
    pos1_alg5_thres = str2double(get(handles.pos1_alg5_thres,'string'));
    plot_thres(pos1_alg5_thres, '    threshold', '[0.2734 0.5078 0.7031]', 'right', handles);
    send_thres(pos1_alg5_thres, 5, handles);
    
function pos1_alg6_thres = pos1_alg6_thres_Callback(hObject, eventdata, handles)
    pos1_alg6_thres = str2double(get(handles.pos1_alg6_thres,'string'));
    plot_thres(pos1_alg6_thres, '    threshold', 'black', 'left', handles);
    send_thres(pos1_alg6_thres, 6, handles);
    
function pos1_alg7_thres = pos1_alg7_thres_Callback(hObject, eventdata, handles)
    pos1_alg7_thres = str2double(get(handles.pos1_alg7_thres,'string'));
    plot_thres(pos1_alg7_thres, '    threshold', '[1 0.6 0]', 'right', handles);
    send_thres(pos1_alg7_thres, 7, handles);
        
function pos1_alg8_thres = pos1_alg8_thres_Callback(hObject, eventdata, handles)
    pos1_alg8_thres = str2double(get(handles.pos1_alg8_thres,'string'));
    plot_thres(pos1_alg8_thres, '    threshold', '[0 0.5 0]', 'right', handles);
    send_thres(pos1_alg8_thres, 8, handles);
    
function pos1_alg9_thres = pos1_alg9_thres_Callback(hObject, eventdata, handles)
    pos1_alg9_thres = str2double(get(handles.pos1_alg9_thres,'string'));
    plot_thres(pos1_alg9_thres, '    threshold', '[0.75 0.75 0.75]', 'left', handles);  
    send_thres(pos1_alg9_thres, 9, handles);
    
function pos1_alg10_thres = pos1_alg10_thres_Callback(hObject, eventdata, handles)
    pos1_alg10_thres = str2double(get(handles.pos1_alg10_thres,'string'));
    plot_thres(pos1_alg10_thres, '    threshold', '[0 0.5430 0.5430]', 'right', handles);
    send_thres(pos1_alg10_thres, 10, handles);

    
    
    
    
function pos2_alg1_thres = pos2_alg1_thres_Callback(hObject, eventdata, handles)
    pos2_alg1_thres = str2double(get(handles.pos2_alg1_thres,'string'));
    plot_thres(pos2_alg1_thres, '    threshold', 'blue', 'left', handles);
    send_thres(pos2_alg1_thres, 11, handles);
    
function pos2_alg2_thres = pos2_alg2_thres_Callback(hObject, eventdata, handles)
    pos2_alg2_thres = str2double(get(handles.pos2_alg2_thres,'string'));
    plot_thres(pos2_alg2_thres, '    threshold', 'red', 'right', handles);
    send_thres(pos2_alg2_thres, 12, handles);
    
function pos2_alg3_thres = pos2_alg3_thres_Callback(hObject, eventdata, handles)
    pos2_alg3_thres = str2double(get(handles.pos2_alg3_thres,'string'));
    plot_thres(pos2_alg3_thres, '    threshold', 'cyan', 'left', handles);
    send_thres(pos2_alg3_thres, 13, handles);
    
function pos2_alg4_thres = pos2_alg4_thres_Callback(hObject, eventdata, handles)
    pos2_alg4_thres = str2double(get(handles.pos2_alg4_thres,'string'));
    plot_thres(pos2_alg4_thres, '    threshold', 'magenta', 'right', handles); 
    send_thres(pos2_alg4_thres, 14, handles);
    
function pos2_alg5_thres = pos2_alg5_thres_Callback(hObject, eventdata, handles)
    pos2_alg5_thres = str2double(get(handles.pos2_alg5_thres,'string'));
    plot_thres(pos2_alg5_thres, '    threshold', '[0.2734 0.5078 0.7031]', 'right', handles);
    send_thres(pos2_alg5_thres, 15, handles);
    
function pos2_alg6_thres = pos2_alg6_thres_Callback(hObject, eventdata, handles)
    pos2_alg6_thres = str2double(get(handles.pos2_alg6_thres,'string'));
    plot_thres(pos2_alg6_thres, '    threshold', 'black', 'left', handles);
    send_thres(pos2_alg6_thres, 16, handles);
        
function pos2_alg8_thres = pos2_alg8_thres_Callback(hObject, eventdata, handles)
    pos2_alg8_thres = str2double(get(handles.pos2_alg8_thres,'string'));
    plot_thres(pos2_alg8_thres, '    threshold', '[0 0.5 0]', 'right', handles);
    send_thres(pos2_alg8_thres, 18, handles);
    
function pos2_alg9_thres = pos2_alg9_thres_Callback(hObject, eventdata, handles)
    pos2_alg9_thres = str2double(get(handles.pos2_alg9_thres,'string'));
    plot_thres(pos2_alg9_thres, '    threshold', '[0.75 0.75 0.75]', 'left', handles);  
    send_thres(pos2_alg9_thres, 19, handles);
    
function pos2_alg10_thres = pos2_alg10_thres_Callback(hObject, eventdata, handles)
    pos2_alg10_thres = str2double(get(handles.pos2_alg10_thres,'string'));
    plot_thres(pos2_alg10_thres, '    threshold', '[0 0.5430 0.5430]', 'right', handles);
    send_thres(pos2_alg10_thres, 20, handles);
    
function pos3_alg1_thres = pos3_alg1_thres_Callback(hObject, eventdata, handles)
    pos3_alg1_thres = str2double(get(handles.pos3_alg1_thres,'string'));
    plot_thres(pos3_alg1_thres, '    threshold', 'blue', 'left', handles);
    send_thres(pos3_alg1_thres, 21, handles);
    
function pos3_alg2_thres = pos3_alg2_thres_Callback(hObject, eventdata, handles)
    pos3_alg2_thres = str2double(get(handles.pos3_alg2_thres,'string'));
    plot_thres(pos3_alg2_thres, '    threshold', 'red', 'right', handles);
    send_thres(pos3_alg2_thres, 22, handles);
    
function pos3_alg3_thres = pos3_alg3_thres_Callback(hObject, eventdata, handles)
    pos3_alg3_thres = str2double(get(handles.pos3_alg3_thres,'string'));
    plot_thres(pos3_alg3_thres, '    threshold', 'cyan', 'left', handles);
    send_thres(pos3_alg3_thres, 23, handles);
    
function pos3_alg4_thres = pos3_alg4_thres_Callback(hObject, eventdata, handles)
    pos3_alg4_thres = str2double(get(handles.pos3_alg4_thres,'string'));
    plot_thres(pos3_alg4_thres, '    threshold', 'magenta', 'right', handles); 
    send_thres(pos3_alg4_thres, 24, handles);
    
function pos3_alg5_thres = pos3_alg5_thres_Callback(hObject, eventdata, handles)
    pos3_alg5_thres = str2double(get(handles.pos3_alg5_thres,'string'));
    plot_thres(pos3_alg5_thres, '    threshold', '[0.2734 0.5078 0.7031]', 'right', handles);
    send_thres(pos3_alg5_thres, 25, handles);
    
function pos3_alg6_thres = pos3_alg6_thres_Callback(hObject, eventdata, handles)
    pos3_alg6_thres = str2double(get(handles.pos3_alg6_thres,'string'));
    plot_thres(pos3_alg6_thres, '    threshold', 'black', 'left', handles);
    send_thres(pos3_alg6_thres, 26, handles);
    
   
function pos3_alg8_thres = pos3_alg8_thres_Callback(hObject, eventdata, handles)
    pos3_alg8_thres = str2double(get(handles.pos3_alg8_thres,'string'));
    plot_thres(pos3_alg8_thres, '    threshold', '[0 0.5 0]', 'right', handles);
    send_thres(pos3_alg8_thres, 28, handles);
    
function pos3_alg9_thres = pos3_alg9_thres_Callback(hObject, eventdata, handles)
    pos3_alg9_thres = str2double(get(handles.pos3_alg9_thres,'string'));
    plot_thres(pos3_alg9_thres, '    threshold', '[0.75 0.75 0.75]', 'left', handles);  
    send_thres(pos3_alg9_thres, 29, handles);
    
function pos3_alg10_thres = pos3_alg10_thres_Callback(hObject, eventdata, handles)
    pos3_alg10_thres = str2double(get(handles.pos3_alg10_thres,'string'));
    plot_thres(pos3_alg10_thres, '    threshold', '[0 0.5430 0.5430]', 'right', handles);
    send_thres(pos3_alg10_thres, 30, handles);
    
%delay for remote line end communication - corresponds to line length
function pos1_alg7_length = pos1_alg7_length_Callback(hObject, eventdata, handles)
    pos1_alg7_length = str2double(get(handles.pos1_alg7_length,'string'));
    send_length(pos1_alg7_length, handles);    

    
    
function plot_thres(threshold, ch_plotname, ch_color, orientation, handles)
    axes(handles.axes3);
    delete(findobj(gca, 'Type', 'line','-and','LineStyle',':','-and','Color', ch_color,'-and','linewidth',1.5));
    scale_factor_voltage = edit_scale_factor_voltage_Callback(handles); 
    yyaxis(handles.axes3,orientation);    
    if strcmp(orientation, 'left')
        scale_factor = scale_factor_voltage;
    else
        scale_factor = 1; %p.u. plot
    end
    threshold = threshold/scale_factor;
    plot([0,799/50], [threshold, threshold], 'LineStyle',':', 'linewidth',1.5,'Marker','none','Color', ch_color,'DisplayName', ch_plotname);
    grid on; hold on;
    xlabel('time [ms]');
    xlim([0 16]);
    legend('-DynamicLegend');
    legend('Location','northeastoutside');
    
function send_thres(threshold, algorithm, handles)
    % alg1: dv/dt                           - 0...320kV
    % alg2: dv/dt + current direction       - 1...5 pu
    % alg3: TW extraction (local)           - 0...320kV
    % alg4: undervoltage                    - 0...1 pu
    % alg5: overcurrent                     - 1...5 pu
    % alg6: TW + current directional (double-ended)
    % alg7: current differential (double-ended) - 0...1 kA
    % alg8: Busbar                          - 0...1 kA
    % alg9: backup: breaker failure
    % alg10:backup (local)    
    
%   first letter "t" marks that it is a threshold to be set
% 	second letter marks the algorithm to be set   1-a for alg1-alg10 in pos1
% 	                                              b-k for alg1-alg10 in pos2
%                                                 l-u for alg1-alg10 in pos3
% 	third letter marks the threshold
    handles.wait_text.BackgroundColor = 'r';
    handles.wait_text.String = 'WAIT';
    
    range = [320 4 320 1 4 0 1 1 0 0];
    if ((algorithm==2)|(algorithm==5)|(algorithm==12)|(algorithm==15)|(algorithm==22)|(algorithm==25))
        threshold = threshold-1;
    end
    
    
    algorithm_num = mod(algorithm,10);
    
    algorithm_letter = {'1' '2' '3' '4' '5' '6' '7' '8' '9' 'a' 'b' 'c' 'd' 'e' 'f' 'g' 'h' 'i' 'j' 'k' 'l' 'm' 'n' 'o' 'p' 'q' 'r' 's' 't' 'u'};
    a = char(algorithm_letter(algorithm));

    
    num_thresholds = 50;
    index = round(num_thresholds*(threshold/range(algorithm_num)));
    if index>49
        index = 49;
    elseif index<1
        index = 1;
    end    
    % 50 threshold values from smallest to highest
    % the value sent corresponds to the chosen threshold within the
    % indicated range in the GUI. The IED then transforms that value into
    % p.u., bits or whatever format is required.
    
    threshold_array = [ ['t' a 'A'];['t' a 'B'];['t' a 'C'];['t' a 'D'];['t' a 'E'];['t' a 'F'];['t' a 'G'];['t' a 'H'];['t' a 'I'];['t' a 'J'];...
                        ['t' a 'K'];['t' a 'L'];['t' a 'M'];['t' a 'N'];['t' a 'O'];['t' a 'P'];['t' a 'Q'];['t' a 'R'];['t' a 'S'];['t' a 'T'];...
                        ['t' a 'U'];['t' a 'V'];['t' a 'W'];['t' a 'X'];['t' a 'Y'];['t' a 'Z'];['t' a 'a'];['t' a 'b'];['t' a 'c'];['t' a 'd'];...
                        ['t' a 'e'];['t' a 'f'];['t' a 'g'];['t' a 'h'];['t' a 'i'];['t' a 'j'];['t' a 'k'];['t' a 'l'];['t' a 'm'];['t' a 'n'];...
                        ['t' a 'o'];['t' a 'p'];['t' a 'q'];['t' a 'r'];['t' a 's'];['t' a 't'];['t' a 'u'];['t' a 'v'];['t' a 'w'];['t' a 'x']];             
    threshold_string = threshold_array(index+1,1:3);
    delete(instrfindall);
    SerialPort=edit_COM_Callback(handles);
    s = serial(SerialPort);
    set(s, 'InputBufferSize', 1024);
    set(s, 'BaudRate', 115200);
    set(s, 'Parity', 'none');
    set(s, 'DataBits', 8);
    set(s, 'StopBit', 1);
    set(s, 'Timeout', 1);
    fopen(s);
    while(1)
         try fprintf(s,threshold_string)
         catch
         end
        if(s.BytesAvailable~=0)
            break;
        end
    end
    while(s.BytesAvailable~=0)
        dummy = fread(s,1,'uint32');
    end 
    handles.wait_text.BackgroundColor = 'g';
    handles.wait_text.String = 'OK';
    

function send_length(length, handles)    
%   first letter "l" marks that it is a length to be set
% 	second letter "l" marks that it is a length to be set
% 	third letter marks the length

    handles.wait_text.BackgroundColor = 'r';
    handles.wait_text.String = 'WAIT';
    
    range = 200; %km
    num = 50;
    index = round(num*(length/range));
    if index>49
        index = 49;
    elseif index<1
        index = 1;
    end    
    % 50 length values from smallest to highest
    % the value sent corresponds to the chosen length within the
    % indicated range in the GUI. The IED then transforms that value into
    % p.u., bits or whatever format is required.
    
    length_array = [ ['l' 'l' 'A'];['l' 'l' 'B'];['l' 'l' 'C'];['l' 'l' 'D'];['l' 'l' 'E'];['l' 'l' 'F'];['l' 'l' 'G'];['l' 'l' 'H'];['l' 'l' 'I'];['l' 'l' 'J'];...
                     ['l' 'l' 'K'];['l' 'l' 'L'];['l' 'l' 'M'];['l' 'l' 'N'];['l' 'l' 'O'];['l' 'l' 'P'];['l' 'l' 'Q'];['l' 'l' 'R'];['l' 'l' 'S'];['l' 'l' 'l'];...
                     ['l' 'l' 'U'];['l' 'l' 'V'];['l' 'l' 'W'];['l' 'l' 'X'];['l' 'l' 'Y'];['l' 'l' 'Z'];['l' 'l' 'a'];['l' 'l' 'b'];['l' 'l' 'c'];['l' 'l' 'd'];...
                     ['l' 'l' 'e'];['l' 'l' 'f'];['l' 'l' 'g'];['l' 'l' 'h'];['l' 'l' 'i'];['l' 'l' 'j'];['l' 'l' 'k'];['l' 'l' 'l'];['l' 'l' 'm'];['l' 'l' 'n'];...
                     ['l' 'l' 'o'];['l' 'l' 'p'];['l' 'l' 'q'];['l' 'l' 'r'];['l' 'l' 's'];['l' 'l' 'l'];['l' 'l' 'u'];['l' 'l' 'v'];['l' 'l' 'w'];['l' 'l' 'x']];             
    length_string = length_array(index+1,1:3);
    delete(instrfindall);
    SerialPort=edit_COM_Callback(handles);
    s = serial(SerialPort);
    set(s, 'InputBufferSize', 1024);
    set(s, 'BaudRate', 115200);
    set(s, 'Parity', 'none');
    set(s, 'DataBits', 8);
    set(s, 'StopBit', 1);
    set(s, 'Timeout', 1);
    fopen(s);
    while(1)
         try fprintf(s,length_string)
         catch
         end
        if(s.BytesAvailable~=0)
            break;
        end
    end
    while(s.BytesAvailable~=0)
        dummy = fread(s,1,'uint32');
    end 
    handles.wait_text.BackgroundColor = 'g';
    handles.wait_text.String = 'OK';
    

    


    
    

function pos1_alg1_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos1_alg2_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos1_alg3_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos1_alg4_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos1_alg5_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos1_alg6_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos1_alg7_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos1_alg7_length_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos1_alg8_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos1_alg9_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos1_alg10_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos2_alg1_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos2_alg4_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos2_alg2_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos2_alg3_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos2_alg5_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos2_alg6_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos2_alg7_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos2_alg8_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos2_alg9_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos2_alg10_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos3_alg1_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos3_alg2_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos3_alg3_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos3_alg4_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos3_alg5_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos3_alg6_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos3_alg7_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos3_alg8_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos3_alg9_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
function pos3_alg10_thres_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% READ TRIP SIGNALS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% //alg1: dv/dt
% //alg2: dv/dt + current direction
% //alg3: TW extraction (local)
% //alg4: undervoltage
% //alg5: overcurrent
% //alg6: TW + current directional (double-ended)
% //alg7: TW + current differential (double-ended)
% //alg8: Busbar
% //alg9: backup: breaker failure
% //alg10:backup (local)

% --- Executes on button press in read_trip_pos.
function read_trip_pos_Callback(hObject, eventdata, handles) 
    handles.wait_text.BackgroundColor = 'r';
    handles.wait_text.String = 'WAIT';
    ch_length = 800;
    trip_times = zeros(4, ch_length);   
    fault_detected_pos = {'fmm','f1p','f2p','f3p'};
    
    for i = 1:4 
        delete(instrfindall);
        SerialPort=edit_COM_Callback(handles);
        s = serial(SerialPort);
        set(s, 'InputBufferSize', 1024);
        set(s, 'BaudRate', 115200);
        set(s, 'Parity', 'none');
        set(s, 'DataBits', 8);
        set(s, 'StopBit', 1);
        set(s, 'Timeout', 1);
        fopen(s);
        while(1)
            string = fault_detected_pos(i);
            try fprintf(s,string{1})
            catch
            end
            if(s.BytesAvailable~=0)
                break;
            end
        end
        step = 1;
        while step < ch_length
            if(s.BytesAvailable~=0)        
                trip_times(i,step) = fread(s,1,'uint32');
                step = step+1;    
            end
        end
    end
    
%     trip_times has format
%     manual 0:800, 0 or 1
%     pos1 0:800, number indicates algorithm that tripped
%     pos2 0:800, number indicates algorithm that tripped
%     pos3 0:800, number indicates algorithm that tripped
    
    trip_times_binary_pos1 = de2bi(trip_times(2,:),10);
    trip_times_binary_pos2 = de2bi(trip_times(3,:),10);
    trip_times_binary_pos3 = de2bi(trip_times(4,:),10);

    axes(handles.axes4);  
    set(handles.axes4,'Position',[0.31 0.049 0.518 0.206]);
    handles.axes4.ActivePositionProperty = 'position';  
    
    %read algorithms    
    fault_detected_1 = [1 1 1 1 1 1 1 1 1 1 1]*800;
    fault_detected_2 = [1 1 1 1 1 1 1 1 1 1 1]*800;
    fault_detected_3 = [1 1 1 1 1 1 1 1 1 1 1]*800;
    %read manual trip
    try fault_detected_1(1) = min(find(trip_times(1,:)));
    catch fault_detected_1(1) = 800; 
    end
    fault_detected_2(1) = fault_detected_1(1);
    fault_detected_3(1) = fault_detected_1(1);
    
    for i=1:10
        try fault_detected_1(i+1) = min(find(trip_times_binary_pos1(:,i)));
        catch fault_detected_1(i+1) = 800;
        end
        try fault_detected_2(i+1) = min(find(trip_times_binary_pos2(:,i)));
        catch fault_detected_2(i+1) = 800;
        end
        try fault_detected_3(i+1) = min(find(trip_times_binary_pos3(:,i)));
        catch fault_detected_3(i+1) = 800;
        end
    end 
    
%     c = categorical({'manual','dv/dt', 'dv/dt + current direction', 'TW extraction (local)', 'undervoltage', 'overcurrent', 'TW + current directional (double-ended)',...
%         'TW + current differential (double-ended)', 'Busbar','backup: breaker failure', 'backup (local)'});  
%     c = reordercats(c,{'backup (local)','backup: breaker failure','Busbar','TW + current differential (double-ended)',...
%         'TW + current directional (double-ended)','overcurrent','undervoltage','TW extraction (local)','dv/dt + current direction','dv/dt', 'manual'});
%     
    c = categorical({'manual','dv/dt', 'dv/dt + current direction', 'TW extraction (local)', 'undervoltage', 'overcurrent', '.','..', 'Busbar','...', '....'});  
    c = reordercats(c,{'....','...','Busbar','..','.','overcurrent','undervoltage','TW extraction (local)','dv/dt + current direction','dv/dt', 'manual'});
    
    fault_detected = [fault_detected_1;fault_detected_2;fault_detected_3];
    handle = barh(c, fault_detected'/800*16,'Basevalue',16);
    set( handle, {'DisplayName'}, {'fault detected link 1p'; 'fault detected link 2p';'fault detected link 3p'} );
    legend show;
    
    grid on;
    xlabel('time [ms]');
    xlim([0 16]);
    legend('-DynamicLegend');
    legend('Location','northeastoutside');
    
    handles.wait_text.BackgroundColor = 'g';
    handles.wait_text.String = 'OK';

% --- Executes on button press in read_trip_neg.
function read_trip_neg_Callback(hObject, eventdata, handles)
    handles.wait_text.BackgroundColor = 'r';
    handles.wait_text.String = 'WAIT';
    ch_length = 800;
    trip_times = zeros(4, ch_length);   
    fault_detected_neg = {'fmm','f1n','f2n','f3n'};
    
    for i = 1:4 
        delete(instrfindall);
        SerialPort=edit_COM_Callback(handles);
        s = serial(SerialPort);
        set(s, 'InputBufferSize', 1024);
        set(s, 'BaudRate', 115200);
        set(s, 'Parity', 'none');
        set(s, 'DataBits', 8);
        set(s, 'StopBit', 1);
        set(s, 'Timeout', 1);
        fopen(s);
        while(1)
            string = fault_detected_neg(i);
            try fprintf(s,string{1})
            catch
            end
            if(s.BytesAvailable~=0)
                break;
            end
        end
        step = 1;
        while step < ch_length
            if(s.BytesAvailable~=0)        
                trip_times(i,step) = fread(s,1,'uint32');
                step = step+1;    
            end
        end
    end
    
%     trip_times has format
%     manual 0:800, 0 or 1
%     pos1 0:800, number indicates algorithm that tripped
%     pos2 0:800, number indicates algorithm that tripped
%     pos3 0:800, number indicates algorithm that tripped
    
    trip_times_binary_neg1 = de2bi(trip_times(2,:),10);
    trip_times_binary_neg2 = de2bi(trip_times(3,:),10);
    trip_times_binary_neg3 = de2bi(trip_times(4,:),10);

    axes(handles.axes4);  
    set(handles.axes4,'Position',[0.31 0.049 0.518 0.206]);
    handles.axes4.ActivePositionProperty = 'position';  
    
    %read algorithms    
    fault_detected_1 = [1 1 1 1 1 1 1 1 1 1 1]*800;
    fault_detected_2 = [1 1 1 1 1 1 1 1 1 1 1]*800;
    fault_detected_3 = [1 1 1 1 1 1 1 1 1 1 1]*800;
    %read manual trip
    try fault_detected_1(1) = min(find(trip_times(1,:)));
    catch fault_detected_1(1) = 800; 
    end
    fault_detected_2(1) = fault_detected_1(1);
    fault_detected_3(1) = fault_detected_1(1);
    
    for i=1:10
        try fault_detected_1(i+1) = min(find(trip_times_binary_neg1(:,i)));
        catch fault_detected_1(i+1) = 800;
        end
        try fault_detected_2(i+1) = min(find(trip_times_binary_neg2(:,i)));
        catch fault_detected_2(i+1) = 800;
        end
        try fault_detected_3(i+1) = min(find(trip_times_binary_neg3(:,i)));
        catch fault_detected_3(i+1) = 800;
        end
    end 
    
%     c = categorical({'manual','dv/dt', 'dv/dt + current direction', 'TW extraction (local)', 'undervoltage', 'overcurrent', 'TW + current directional (double-ended)',...
%         'TW + current differential (double-ended)', 'Busbar','backup: breaker failure', 'backup (local)'});  
%     c = reordercats(c,{'backup (local)','backup: breaker failure','Busbar','TW + current differential (double-ended)',...
%         'TW + current directional (double-ended)','overcurrent','undervoltage','TW extraction (local)','dv/dt + current direction','dv/dt', 'manual'});
    
    c = categorical({'manual','dv/dt', 'dv/dt + current direction', 'TW extraction (local)', 'undervoltage', 'overcurrent', '.','..', 'Busbar','...', '....'});  
    c = reordercats(c,{'....','...','Busbar','..','.','overcurrent','undervoltage','TW extraction (local)','dv/dt + current direction','dv/dt', 'manual'});

    fault_detected = [fault_detected_1;fault_detected_2;fault_detected_3];
    handle = barh(c, fault_detected'/800*16,'Basevalue',16);
    set( handle, {'DisplayName'}, {'fault detected link 1n'; 'fault detected link 2n';'fault detected link 3n'} );
    legend show;
    
    grid on;
    xlabel('time [ms]');
    xlim([0 16]);
    legend('-DynamicLegend');
    legend('Location','northeastoutside');
    
    handles.wait_text.BackgroundColor = 'g';
    handles.wait_text.String = 'OK';
   


% --- Executes on button press in save_data.
function save_data_Callback(hObject, eventdata, handles)
% hObject    handle to save_data (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    data = zeros(34,801);
    if (handles.show_ch0.Value) yyaxis(handles.axes2,'left');
        for i = 1:16
            string = handles.axes2.Children(i).DisplayName;            
            if(strfind(string,'1p - voltage')) data(1) = ['1p - voltage' handles.axes2.Children(i).YData];            end           
            if(strfind(string,'1p - voltage - filtered')) data(2) = ['1p - voltage - filtered',handles.axes2.Children(i).YData];            end           
        end        
    end
    
    
   

function voltage_upper_limit = voltage_upper_limit_Callback(handles)
voltage_upper_limit = str2num(handles.voltage_upper_limit.String);


function voltage_lower_limit = voltage_lower_limit_Callback(handles)
voltage_lower_limit = str2num(handles.voltage_lower_limit.String);

% function current_upper_limit_Callback(handles)
 function current_upper_limit = current_upper_limit_Callback(handles)
current_upper_limit = str2num(handles.current_upper_limit.String);

% function current_lower_limit_Callback(handles)
function current_lower_limit = current_lower_limit_Callback(handles)
current_lower_limit = str2num(handles.current_lower_limit.String);

function left_limit_Callback(handles)
% function left_limit = left_limit(handles)
left_limit = str2num(handles.left_limit.String);

function left_limit_lower_Callback(handles)
% function left_limit_lower = left_limit_lower(handles)
left_limit_lower = str2num(handles.left_limit_lower.String);










function voltage_lower_limit_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function current_upper_limit_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function current_lower_limit_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function left_limit_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function left_limit_lower_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function voltage_upper_limit_CreateFcn(hObject, eventdata, handles)
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in only_filtered.
function only_filtered = only_filtered_Callback(hObject, eventdata, handles)
if (get(hObject,'Value') == get(hObject,'Max'))
    only_filtered = 1;
else
    only_filtered = 0;
end
