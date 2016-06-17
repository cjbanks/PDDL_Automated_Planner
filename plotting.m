function plotting()
clc; close all;
addpath('Strips','Numeric','Time');

% CHANGE FILES HERE
%  plotting_call('Depot_Strips.txt','DriverLog_Strips.txt',...
%         'ZenoTravels_Strips.txt');
%     
% plotting_call('Depot_Numeric.txt','DriverLog_Numeric.txt',...
%         'ZenoTravel_Numeric.txt');

% plotting_call('Depot_Time.txt','DriverLog_Time.txt',...
%         'ZenoTravel_Time.txt');

% plotting_call('Depot_Strips.txt','Depot_Numeric.txt',...
%         'Depot_Time.txt');

% plotting_call('DriverLog_Strips.txt','DriverLog_Numeric.txt',...
%         'DriverLog_Time.txt');

plotting_call('ZenoTravel_Strips.txt','ZenoTravel_Numeric.txt',...
        'ZenoTravel_Time.txt');

end


function [time, cost, numSubPlans, numGoals] = getData(filename)

% Read files
fid = fopen(filename);
a = textscan(fid, '%s %s %s %s %s');
a = horzcat(a{:});
a = a(2:end,:);
fclose(fid);

% Get metrics
numSubPlans = cellfun(@str2num,a(:,2));
numGoals = cellfun(@str2num,a(:,3));
time = cellfun(@str2num,a(:,4));
cost = cellfun(@str2num,a(:,5));

% Ordering by time
[time, idx] = sort(time);
cost = cost(idx);
numSubPlans = numSubPlans(idx);
numGoals = numGoals(idx);

end


function plotting_call(file1, file2, file3)

[time_1, cost_1, numSubPlans_1, numGoals_1] = getData(file1);
[time_2, cost_2, numSubPlans_2, numGoals_2] = getData(file2);
[time_3, cost_3, numSubPlans_3, numGoals_3] = getData(file3);

% Plotting
figure;
plot(time_1,'.c-','markersize',16); hold on;
plot(time_2,'*r-','markersize',8); 
plot(time_3,'^b-','markersize',6);
xlabel('Problem files','fontsize',14);
ylabel('Computation Time [sec]','fontsize',14);
title('Complexity Comparison (ZenoTravel)','fontsize',16);      % CHANGE TITLE HERE
lhandle = legend('Strips','Numeric','Time',...  % CHANGE LEGEND HERE
    'location','northwest');  
set(lhandle, 'fontsize',12)


% Printing Metrics
% --Remove Nans first
time_1 = time_1(~isnan(time_1));
time_2 = time_2(~isnan(time_2));
time_3 = time_3(~isnan(time_3));
disp(['Median value of ',file1,' = ',num2str(median(time_1))]);
disp(['Median value of ',file2,' = ',num2str(median(time_2))]);
disp(['Median value of ',file3,' = ',num2str(median(time_3))]);

end



