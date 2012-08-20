function [ nr ] = exportBESTdata( se, sites, number, mode)
%exportBESTdata exports BEST data to data files

len= length(se);
nr= min(number, len);

filename= strcat('se', num2str(nr), '.dat');

fid= fopen(filename, 'wt');
fprintf(fid, '%d\n', nr);

if strcmp(mode, 'seq')
    disp('sequential mode...');
    rndmap= 1:nr;
else
    disp('randomized mode...');
    rndmap= randi(len, nr, 1);
end

for i=1:nr
    n= length(se(rndmap(i)).data);
    data= se(rndmap(i)).data;
    dates= se(rndmap(i)).dates;
    unc= se(rndmap(i)).unc;
    tob= se(rndmap(i)).tob;
    num= se(rndmap(i)).num;
    fprintf(fid, '%d\n', n);
    for j=1:n
        fprintf(fid, '%f %f %f %d %d\n', dates(j), data(j), unc(j), tob(j), num(j));
    end
end

filename= strcat('sites', num2str(nr), '.dat');

fid= fopen(filename, 'wt');
fprintf(fid, '%d\n', nr);

for i=1:nr         
    fprintf(fid, '%f %f %f\n', sites(rndmap(i)).latitude, sites(rndmap(i)).longitude, sites(rndmap(i)).elevation);
    relocated= sites(rndmap(i)).relocated;
    sz= size(relocated);
    fprintf(fid, '%d\n', sz(2));
    if ( sz(2) > 0 )
        for j=1:sz(2)
            fprintf(fid, '%f ', relocated(j));
        end
        fprintf(fid, '\n');
    end
    
    possible_relocated= sites(rndmap(i)).possible_relocated;
    sz= size(possible_relocated);
    fprintf(fid, '%d\n', sz(2));
    if ( sz(2) > 0 )
        for j=1:sz(2)
            fprintf(fid, '%f ', possible_relocated(j));
        end
        fprintf(fid, '\n');
    end
end
    
