z = instrfind;
if (~isempty(z))
    fclose(z);
    delete(z);
    freeserial;
end