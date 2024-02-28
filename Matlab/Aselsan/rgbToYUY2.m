function yuy2Image = rgbToYUY2(rgbImage)
    % Convert RGB image to YCbCr color space
    ycbcrImage = rgb2ycbcr(rgbImage);
    
    % Preallocate the YUY2 image
    [height, width, ~] = size(rgbImage);
    yuy2Image = zeros(height, width * 2, 'uint8');

    % Extract Y, U, and V channels
    Y = ycbcrImage(:,:,1);
    U = ycbcrImage(:,:,2);
    V = ycbcrImage(:,:,3);

    % Subsample U and V channels by 2 horizontally
    U = 0.5 * (U(:,1:2:end) + U(:,2:2:end));
    V = 0.5 * (V(:,1:2:end) + V(:,2:2:end));

    % Pack the YUY2 image: Y0 U Y1 V
    yuy2Image(:, 1:4:end) = Y(:, 1:2:end);  % Y0
    yuy2Image(:, 2:4:end) = U;              % U (for Y0 and Y1)
    yuy2Image(:, 3:4:end) = Y(:, 2:2:end);  % Y1
    yuy2Image(:, 4:4:end) = V;              % V (for Y0 and Y1)
end