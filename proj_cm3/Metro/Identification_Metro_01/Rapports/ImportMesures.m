%function ImportMesures(file,prefix)
% file = 'nom du fichier'

function ImportMesures(file,prefix)
%IMPORTFILE(FILETOREAD1)
%  Imports data from the specified file
%  FILETOREAD1:  file to read

%  Auto-generated by MATLAB on 13-Oct-2009 14:57:17

display ('Import des fichiers CSV ...');
% Import les donn�es du train 1
% Donn�es li�es � l'accelaration avant
newData = importdata(file);

% Affect les donn�es.
for i=1:6
    varname = [prefix newData.colheaders{i}];
    disp(['create ' varname]);
    if (varname(end)==' ') varname=varname(1:(end-1));end
    assignin ('base',varname , newData.data(:, i));
end




